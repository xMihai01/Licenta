#include "Model/Utils.h"

std::pair<cv::Mat, cv::Mat> Utils::GetHistograms(const cv::Mat grayImage)
{
	if (grayImage.channels() != 1)
		throw std::runtime_error("GetHistogram() requires a gray image, but it has been given a color image");

	int histogramSize = 256;
	float range[] = { 0, 256 };
	const float* histogramRange = { range };

	cv::Mat histogram;

	cv::calcHist(&grayImage, 1, 0, cv::Mat(), histogram, 1, &histogramSize, &histogramRange);

	cv::Mat cumulativeHistogram(histogram.rows, histogram.cols, histogram.type());

	cumulativeHistogram.at<float>(0) = histogram.at<float>(0);
	for (int i = 1; i < histogramSize; i++) {
		cumulativeHistogram.at<float>(i) = cumulativeHistogram.at<float>(i - 1) + histogram.at<float>(i);
	}

	cv::normalize(histogram, histogram, 0, grayImage.rows, cv::NORM_MINMAX, -1);
	cv::normalize(cumulativeHistogram, cumulativeHistogram, 0, grayImage.rows, cv::NORM_MINMAX, -1);

	int binWidth = cvRound((double)HISTOGRAM_WIDTH / histogramSize);

	cv::Mat histogramImage(HISTOGRAM_HEIGHT, HISTOGRAM_WIDTH, CV_8UC3, cv::Scalar(0, 0, 0));
	cv::Mat cumulativeHistogramImage(HISTOGRAM_HEIGHT, HISTOGRAM_WIDTH, CV_8UC3, cv::Scalar(0, 0, 0));


	for (int i = 1; i < histogramSize; i++) {
		cv::line(histogramImage,
			cv::Point(binWidth * (i - 1), HISTOGRAM_HEIGHT - cvRound(histogram.at<float>(i - 1))),
			cv::Point(binWidth * (i), HISTOGRAM_HEIGHT - cvRound(histogram.at<float>(i))),
			cv::Scalar(255, 255, 255), 2, 8, 0);
	
		cv::line(cumulativeHistogramImage,
			cv::Point(binWidth * (i - 1), HISTOGRAM_HEIGHT - cvRound(cumulativeHistogram.at<float>(i - 1))),
			cv::Point(binWidth * (i), HISTOGRAM_HEIGHT - cvRound(cumulativeHistogram.at<float>(i))),
			cv::Scalar(0, 0, 255), 2, 8, 0);
		
	}
	return std::make_pair(histogramImage, cumulativeHistogramImage);
}

void Utils::GetImageByHighestContour(const cv::Mat& inputImage, cv::Mat& outputImage, std::vector<cv::Point>& maxContour
	, const bool crop, const std::vector<std::vector<cv::Point>>& customContours)
{
	if ((inputImage.empty() || outputImage.empty()) && crop == true)
		throw std::runtime_error("Can't crop given images. They are empty!");

	std::vector<std::vector<cv::Point>> contours;
	customContours.size() == 0 ? cv::findContours(inputImage, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE) : contours = customContours;

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

void Utils::BitwiseCharImage(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst)
{
	if (src1.size != src2.size)
		throw std::runtime_error("Src1 and src2 images are of different sizes!");
	dst = src1.clone();
	for (int i = 0; i < src1.rows; i++) {
		const uchar* ptr1 = src1.ptr<uchar>(i);
		const uchar* ptr2 = src2.ptr<uchar>(i);
		uchar* ptrDst = dst.ptr<uchar>(i);

		for (int j = 0; j < src1.cols; j++)
		{
			const uchar* pixelSrc1 = ptr1 + j;
			const uchar* pixelSrc2 = ptr2 + j;
			uchar pixelValueSrc1 = pixelSrc1[0] <= 254 ? 0 : 255;
			uchar pixelValueSrc2 = pixelSrc2[0] <= 254 ? 0 : 255;
			uchar* pixelDst = ptrDst + j;

			pixelValueSrc1 == pixelValueSrc2 ? ((pixelValueSrc1 == 0) ? pixelDst[0] = 0 : pixelDst[0] = 255) : pixelDst[0] = 0;

		}
	}
}

void Utils::BitwiseLicensePlateImage(const cv::Mat& blankLicensePlate, const cv::Mat& thresholdedLicensePlate, cv::Mat& outputPlate)
{
	if (blankLicensePlate.size != thresholdedLicensePlate.size)
		throw std::runtime_error("Src1 and src2 images are of different sizes!");
	outputPlate = blankLicensePlate.clone();
	for (int i = 0; i < blankLicensePlate.rows; i++) {
		const uchar* blankPlatePtr = blankLicensePlate.ptr<uchar>(i);
		const uchar* threshPlatePtr = thresholdedLicensePlate.ptr<uchar>(i);
		uchar* outputPtr = outputPlate.ptr<uchar>(i);

		for (int j = 0; j < blankLicensePlate.cols; j++)
		{
			const uchar* pixelBlankPlate = blankPlatePtr + j;
			const uchar* pixelThreshPlate = threshPlatePtr + j;
			uchar pixelValueBlankPlate = pixelBlankPlate[0] <= 254 ? 0 : 255;
			uchar pixelValueThreshPlate = pixelThreshPlate[0] <= 254 ? 0 : 255;
			uchar* pixelOutputPlate = outputPtr + j;

			(pixelValueBlankPlate == 255 && pixelValueThreshPlate == 0) ? pixelOutputPlate[0] = 0 : pixelOutputPlate[0] = 255;

		}

	}
}

bool Utils::letterLocationComparator(std::pair<cv::Mat, cv::Rect>& a, std::pair<cv::Mat, cv::Rect>& b)
{
	return a.second.x < b.second.x;
}

bool Utils::pointComparatorByX(cv::Point& a, cv::Point& b) {
	return a.x > b.x;
}

std::vector<std::string> Utils::GetImageNamesFromFile(const std::string& path)
{
	std::vector<std::string> imageNames;
	std::ifstream file(path);

	std::string imageName;

	while (file >> imageName)
		imageNames.push_back(imageName);
	
	return imageNames;
}
