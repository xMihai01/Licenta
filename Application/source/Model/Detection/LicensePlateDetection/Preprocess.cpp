#include "Model/Detection/LicensePlateDetection/Preprocess.h"

bool pointComparatorByX(cv::Point& a, cv::Point& b) {
	return a.x > b.x;
}

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
	NoiseReduction(outputImage, outputImage, Gaussian);

	ResizeImage(outputImage, outputImage, Utils::PLATE_WIDTH, Utils::PLATE_HEIGHT);

	cv::threshold(outputImage, outputImage, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);
	
	std::vector<cv::Point> max_cnt;

	Utils::GetImageByHighestContour(outputImage, outputImage, max_cnt);
	//cv::drawContours(inputImage, std::vector<std::vector<cv::Point>>{max_cnt}, -1, cv::Scalar(0, 255, 0), 1);
	// 
	//m_preprocessing.ConvertImageToGray(outputImage, outputImage);

	cv::bitwise_not(outputImage, outputImage);
	//cv::threshold(outputImage, outputImage, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);

	cv::RotatedRect rotatedRect = cv::minAreaRect(max_cnt);
	float angle = rotatedRect.angle;

	if (angle > 85 && angle < 5) // image doesn't need skew correction
		return;

	int h = outputImage.rows;
	int w = outputImage.cols;
	cv::Point2f center(w / 2, h / 2);
	cv::Mat rotationMatrix = cv::getRotationMatrix2D(center, angle < 45 ? angle : angle - 90, 1.0); // bottom-left down - 90. bottom-left up + 0

	cv::warpAffine(outputImage, outputImage, rotationMatrix, cv::Size(w, h), cv::INTER_CUBIC, cv::BORDER_REPLICATE);
	cv::bitwise_not(outputImage, outputImage);

	Utils::GetImageByHighestContour(outputImage, outputImage, max_cnt, true);
}

void LicensePlateDetection::Preprocess::Undistortion(cv::Mat& inputImage, cv::Mat& outputImage)
{
	cv::Mat licensePlate;
	ConvertImageToGray(inputImage, inputImage);
	ResizeImage(inputImage, inputImage, Utils::PLATE_WIDTH, Utils::PLATE_HEIGHT);
	cv::threshold(inputImage, inputImage, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);
	licensePlate = inputImage.clone();
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(6, 6));
	//cv::dilate(licensePlate, licensePlate, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(6, 6)));
	//cv::erode(licensePlate, licensePlate, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(4, 4)));


	std::vector<cv::Point> max_cnt;

	Utils::GetImageByHighestContour(licensePlate, licensePlate, max_cnt);
	cv::drawContours(licensePlate, std::vector<std::vector<cv::Point>>{max_cnt}, -1, cv::Scalar(255), cv::FILLED);
	cv::erode(licensePlate, licensePlate, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7)));
	cv::dilate(licensePlate, licensePlate, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)));
	Utils::GetImageByHighestContour(licensePlate, licensePlate, max_cnt);
	cv::Rect contourRectangle = cv::boundingRect(max_cnt);
	//cv::cvtColor(licensePlate, licensePlate, cv::COLOR_GRAY2BGR);

	double epsilon = 0.02 * cv::arcLength(max_cnt, true);
	std::vector<cv::Point> approx;
	cv::approxPolyDP(max_cnt, approx, epsilon, true);
	//std::sort(approx.begin(), approx.end(), pointComparatorByX);

	std::array<cv::Point, NUMBER_OF_CORNERS_IN_LICENSE_PLATE> corners;
	try {
		corners = GetLicensePlateCornersFromApproximatedCurves(approx);
	}
	catch (const std::exception& e) {
		// TODO: treat exception when corners are not found (prob try skew correction)
		outputImage = licensePlate;
		return;
	}
	
	//for (int i = 0; i < approx.size(); i++) {
	//	cv::circle(licensePlate, approx[i], 5, cv::Scalar(255, 0, 0), -1);
	//}
	//cv::cvtColor(licensePlate, licensePlate, cv::COLOR_GRAY2BGR);
	//for (int i = 0; i < approx.size(); i++) {
	//	cv::circle(licensePlate, approx[i], 5, cv::Scalar(255, 0, 0), -1);
	//}
	//for (int i = 0; i < 2; i++) {
	//	cv::circle(licensePlate, approx[i], 5, cv::Scalar(0, 0, 255), -1);
	//}
	//for (int i = approx.size() - 1; i > approx.size() - 3; i--) {
	//	cv::circle(licensePlate, approx[i], 5, cv::Scalar(0, 0, 255), -1);
	//}
	//outputImage = licensePlate;
	//return;


	std::vector<cv::Point> srcPoints = {
	(corners[3].y < corners[2].y) ? corners[3] : corners[2], // top left
	(corners[0].y < corners[1].y) ? corners[0] : corners[1], // top right
	(corners[0].y < corners[1].y) ? corners[1] : corners[0], // bottom right
	(corners[3].y < corners[2].y) ? corners[2] : corners[3] // bottom left
	};
	std::vector<cv::Point> dstPoints = {
		cv::Point(1.0f / 8.0f * Utils::PLATE_WIDTH, 1.0f / 3.0f * Utils::PLATE_HEIGHT), // top left
		cv::Point(Utils::PLATE_WIDTH - 1.0f / 3.0f * Utils::PLATE_HEIGHT, 1.0f / 3.0f * Utils::PLATE_HEIGHT), // top right
		cv::Point(Utils::PLATE_WIDTH - 1.0f / 3.0f * Utils::PLATE_HEIGHT, 2.0f / 3.0f * Utils::PLATE_HEIGHT), // bottom right
		cv::Point(1.0f / 8.0f * Utils::PLATE_WIDTH, 2.0f / 3.0f * Utils::PLATE_HEIGHT) // bottom left
	};

	cv::Mat H = cv::findHomography(srcPoints, dstPoints, cv::RANSAC);

	cv::Mat cleanPlateImage;
	Utils::BitwiseLicensePlateImage(licensePlate, inputImage, cleanPlateImage);

	cv::warpPerspective(cleanPlateImage, outputImage, H, licensePlate.size());

	//Utils::GetImageByHighestContour(outputImage, outputImage, max_cnt, true);
}

void LicensePlateDetection::Preprocess::DetectEdges(const cv::Mat& inputImage, cv::Mat& outputImage, const int xDirection, const int yDirection)
{
	cv::Sobel(inputImage, outputImage, CV_8U, xDirection, yDirection);
	cv::threshold(outputImage, outputImage, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);
	cv::Mat elementStructure = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
	cv::dilate(outputImage, outputImage, elementStructure, cv::Point(-1, -1), 2);
	cv::erode(outputImage, outputImage, elementStructure);
}

void LicensePlateDetection::Preprocess::GetVerticalEdges(cv::Mat& inputOutputImage)
{
	cv::Sobel(inputOutputImage, inputOutputImage, CV_8U, 1, 0, 3);
	cv::threshold(inputOutputImage, inputOutputImage, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);
	cv::Mat elementStructure = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));
	cv::morphologyEx(inputOutputImage, inputOutputImage, cv::MORPH_CLOSE, elementStructure);
}

std::array<cv::Point, LicensePlateDetection::Preprocess::NUMBER_OF_CORNERS_IN_LICENSE_PLATE> LicensePlateDetection::Preprocess::GetLicensePlateCornersFromApproximatedCurves(std::vector<cv::Point>& approximations)
{
	if (approximations.size() < 4)
		throw std::runtime_error("Couldn't detect corners of license plate using distortion correction. A minimum of 4 corners required but only " + std::to_string(approximations.size()) + " were found");
	
	std::array<cv::Point, NUMBER_OF_CORNERS_IN_LICENSE_PLATE> corners;

	std::sort(approximations.begin(), approximations.end(), pointComparatorByX);

	corners[0] = approximations[0];
	corners[1] = approximations[1];
	corners[2] = approximations[approximations.size() - 1];
	corners[3] = approximations[approximations.size() - 2];

	return corners;
}
