#include "Model/Detection/LicensePlateDetection/Preprocess.h"

LicensePlateDetection::Preprocess::Preprocess()
{

}

void LicensePlateDetection::Preprocess::ResizeImage(const cv::Mat& inputImage, cv::Mat& outputImage, const int width, const int height)
{
	cv::resize(inputImage, outputImage, cv::Size(width, height), 0.0, 0.0, cv::INTER_AREA);
} 

void LicensePlateDetection::Preprocess::ConvertImageToGray(const cv::Mat& inputImage, cv::Mat& outputImage)
{

	if (!inputImage.data) {
		throw std::runtime_error("Invalid input image.");
	}

	cv::Mat grayImage = cv::Mat::zeros(inputImage.rows, inputImage.cols, CV_8UC1);

	for (int i = 0; i < inputImage.rows; i++) {
		const cv::Vec3b* InImage = inputImage.ptr<cv::Vec3b>(i);
		uchar* OutImage = grayImage.ptr<uchar>(i);

		for (int j = 0; j < inputImage.cols; j++)
		{
			uchar blue = InImage[j][0];
			uchar green = InImage[j][1];
			uchar red = InImage[j][2];

			uchar grayPixel = (0.11 * blue + 0.59 * green + 0.3 * red);
			OutImage[j] = grayPixel;
		}
	}
	outputImage = grayImage;
}

void LicensePlateDetection::Preprocess::NoiseReduction(const cv::Mat& inputImage, cv::Mat& outputImage, const SmoothingAlgorithm smoothingAlgorithm)
{
	cv::Mat tempImage;
	switch (smoothingAlgorithm)
	{
	case SmoothingAlgorithm::BilateralFilter:
		cv::bilateralFilter(inputImage, tempImage, DIAMETER, SIGMA_FOR_BILATERAL_FILTER, SIGMA_FOR_BILATERAL_FILTER);
		outputImage = tempImage;
		break;
	case SmoothingAlgorithm::Gaussian:
		cv::GaussianBlur(inputImage, outputImage, BLUR_KERNEL_SIZE, 0.25);
		break;
	case SmoothingAlgorithm::Averaging:
		cv::blur(inputImage, outputImage, BLUR_KERNEL_SIZE);
		break;
	case SmoothingAlgorithm::Median:
		cv::medianBlur(inputImage, outputImage, INTEGER_KERNEL_SIZE);
		break;
	case SmoothingAlgorithm::Stack:
		cv::stackBlur(inputImage, outputImage, BLUR_KERNEL_SIZE);
		break;
	case SmoothingAlgorithm::BoxFilter:
		cv::boxFilter(inputImage, outputImage, -1, cv::Size(5,5));
		break;
	case SmoothingAlgorithm::NonLocalMeans:
		cv::fastNlMeansDenoising(inputImage, outputImage); // too slow
		break;
	default:
		break;
	}
}
void LicensePlateDetection::Preprocess::AdaptiveHistogramEqualization(const cv::Mat& inputImage, cv::Mat& outputImage)
{
	cv::Ptr<cv::CLAHE> adaptiveHE = cv::createCLAHE();
	adaptiveHE->apply(inputImage, outputImage);

	//cv::equalizeHist(inputImage, outputImage);
	
}

void LicensePlateDetection::Preprocess::PreProcessForHaarCascade(const cv::Mat& inputImage, cv::Mat& outputImage)
{
	ConvertImageToGray(inputImage, outputImage);
	NoiseReduction(outputImage, outputImage, SmoothingAlgorithm::Gaussian);


	cv::Mat dilateKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));

	cv::dilate(outputImage, outputImage, dilateKernel);

	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2, 2));
	cv::morphologyEx(outputImage, outputImage, cv::MORPH_OPEN, kernel);

}

void LicensePlateDetection::Preprocess::SkewCorrection(cv::Mat& inputImage, cv::Mat& outputImage)
{
	ConvertImageToGray(inputImage, outputImage);
	ResizeImage(outputImage, outputImage, 300, 100);
	//m_preprocessing.NoiseReduction(outputImage, outputImage, Gaussian);

	cv::threshold(outputImage, outputImage, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);

	std::vector<cv::Point> max_cnt;

	GetImageByHighestContour(outputImage, outputImage, max_cnt);
	//cv::drawContours(inputImage, std::vector<std::vector<cv::Point>>{max_cnt}, -1, cv::Scalar(0, 255, 0), 1);
	// 
	//m_preprocessing.ConvertImageToGray(outputImage, outputImage);

	cv::bitwise_not(outputImage, outputImage);
	//cv::threshold(outputImage, outputImage, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);

	cv::RotatedRect rotatedRect = cv::minAreaRect(max_cnt);
	float angle = rotatedRect.angle;

	if (angle > 86 && angle < 10) // image doesn't need skew correction
		return;

	int h = outputImage.rows;
	int w = outputImage.cols;
	cv::Point2f center(w / 2, h / 2);
	cv::Mat rotationMatrix = cv::getRotationMatrix2D(center, angle < 45 ? angle : angle - 90, 1.0); // bottom-left down - 90. bottom-left up + 0

	cv::warpAffine(outputImage, outputImage, rotationMatrix, cv::Size(w, h), cv::INTER_CUBIC, cv::BORDER_REPLICATE);

	cv::bitwise_not(outputImage, outputImage);

	GetImageByHighestContour(outputImage, outputImage, max_cnt, true);
}

void LicensePlateDetection::Preprocess::GetImageByHighestContour(cv::Mat& inputImage, cv::Mat& outputImage, std::vector<cv::Point>& maxContour, const bool crop)
{
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(inputImage, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

	std::vector<double> areas;
	for (const auto& contour : contours) {
		areas.push_back(cv::contourArea(contour));
	}

	int maxIndex = static_cast<int>(std::distance(areas.begin(), std::max_element(areas.begin(), areas.end())));
	std::vector<cv::Point> max_cnt = contours[maxIndex];

	//cv::drawContours(originalImage, std::vector<std::vector<cv::Point>>{max_cnt}, -1, cv::Scalar(0, 255, 0), 1);

	cv::Rect contourRectangle = cv::boundingRect(max_cnt);
	double x = contourRectangle.x;
	double y = contourRectangle.y;
	double width = contourRectangle.width;
	double height = contourRectangle.height;
	if (crop)
		outputImage = inputImage(cv::Rect(x, y, width, height));
	maxContour = max_cnt;
}

void LicensePlateDetection::Preprocess::DetectEdges(const cv::Mat& inputImage, cv::Mat& outputImage, const double minValue, const double maxValue)
{
	cv::Canny(inputImage, outputImage, minValue, maxValue);
}

void LicensePlateDetection::Preprocess::GetVerticalEdges(cv::Mat& inputOutputImage)
{
	cv::Sobel(inputOutputImage, inputOutputImage, CV_8U, 1, 0, 3);
	cv::threshold(inputOutputImage, inputOutputImage, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);
	cv::Mat elementStructure = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));
	cv::morphologyEx(inputOutputImage, inputOutputImage, cv::MORPH_CLOSE, elementStructure);
}