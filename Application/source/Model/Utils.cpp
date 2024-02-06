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

bool Utils::DoesImageNeedCLAHE(const cv::Mat grayImage)
{

	if (grayImage.channels() != 1)
		throw std::runtime_error("DoesImageNeedCLAHE() requires a gray image, but it has been given a color image");

	double whites = 0;
	double blacks = 0;

	for (int i = 0; i < grayImage.rows; i++) {
		const uchar* grayImagePtr = grayImage.ptr<uchar>(i);

		for (int j = 0; j < grayImage.cols; j++)
		{
			const uchar* pixelValue = grayImagePtr + j;
			if (pixelValue[0] > 128)
				whites += pixelValue[0];
			else
				blacks += pixelValue[0];
		}

		
	}
	std::cout << "\n\n";
	std::cout << whites << " " << blacks;
	return blacks > whites;
}

void Utils::GetImageByHighestContour(const cv::Mat& inputImage, cv::Mat& outputImage, std::vector<cv::Point>& maxContour
	, const bool crop, const std::vector<std::vector<cv::Point>>& customContours)
{
	//if ((inputImage.empty() || outputImage.empty()) && crop == true)
	if (inputImage.empty() && crop == true)
		throw std::runtime_error("Can't crop given images. They are empty!");

	std::vector<std::vector<cv::Point>> contours;
	//customContours.size() == 0 ? cv::findContours(inputImage, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE) : contours = customContours;
	if (customContours.size() == 0)
		cv::findContours(inputImage, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);
	else
		contours = customContours;

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

void Utils::SkeletonizeImage(const cv::Mat& inputImage, cv::Mat& outputImage)
{
	cv::Mat originalImage;
	cv::bitwise_not(inputImage, originalImage);
	//cv::resize(originalImage, originalImage, cv::Size(100, 100));
	cv::Size size = originalImage.size();
	cv::Mat skel = cv::Mat::zeros(originalImage.size(), CV_8U);

	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));
	bool done = false;

	while (!done) {
		cv::Mat eroded;
		cv::erode(originalImage, eroded, kernel);
		cv::Mat temp;
		cv::dilate(eroded, temp, kernel);
		cv::subtract(originalImage, temp, temp);
		cv::bitwise_or(skel, temp, skel);

		originalImage = eroded.clone();

		int zeros = size.width * size.height - cv::countNonZero(originalImage);
		if (zeros == size.width * size.height) {
			done = true;
		}
	}
	outputImage = skel;
}

bool Utils::IsRectangleInsideAnotherRectangle(const cv::Rect& first, const cv::Rect& second) // first rectangle (smaller one) is in second rectangle (bigger one)
{
	bool isXcoordInside = second.x <= first.x && second.x + second.width >= first.x + first.width;
	bool isYcoordInside = second.y <= first.y && second.y + second.height >= first.y + first.height;
	return isXcoordInside && isYcoordInside;
}

bool Utils::letterLocationComparator(std::pair<cv::Mat, cv::Rect>& a, std::pair<cv::Mat, cv::Rect>& b)
{
	return a.second.x < b.second.x;
}

bool Utils::pointComparatorByX(cv::Point& a, cv::Point& b) {
	return a.x > b.x;
}

bool Utils::areaComparatorForContours(std::vector<cv::Point>& a, std::vector<cv::Point>& b)
{
	return cv::contourArea(a) < cv::contourArea(b);
}

void Utils::ShowRectangleOnImage(const cv::Mat& inputImage, cv::Mat& outputImage, const cv::Point2d& first, const cv::Point2d& second)
{
	const cv::Vec3b red = cv::Vec3b(0, 0, 255);
	outputImage = inputImage.clone();
	cv::Point2d max = cv::Point2d(std::max(first.x, second.x), std::max(first.y, second.y));
	cv::Point2d min = cv::Point2d(std::min(first.x, second.x), std::min(first.y, second.y));

	for (int row = static_cast<int>(min.y); row <= static_cast<int>(max.y); row++) {
		for (int col = static_cast<int>(min.x); col <= static_cast<int>(max.x); col++) {
			if ((row == min.y || row == max.y) && (col > min.x && col < max.x)) {
				outputImage.at<cv::Vec3b>(row, col) = red;
			}
			if ((col == min.x || col == max.x) && (row > min.y && row < max.y)) {
				outputImage.at<cv::Vec3b>(row, col) = red;
			}
		}
	}

}

void Utils::CropImageFromRectangle(const cv::Mat& inputImage, cv::Mat& outputImage, const cv::Point2d& first, const cv::Point2d& second)
{
	cv::Point2d max = cv::Point2d(std::max(first.x, second.x), std::max(first.y, second.y));
	cv::Point2d min = cv::Point2d(std::min(first.x, second.x), std::min(first.y, second.y));

	min.x = std::max(0.0, min.x);
	min.y = std::max(0.0, min.y);
	max.x = std::min(static_cast<double>(inputImage.cols - 1), max.x);
	max.y = std::min(static_cast<double>(inputImage.rows - 1), max.y);
	
	outputImage = cv::Mat(cv::Size(max.x - min.x + 1, max.y - min.y + 1), CV_8UC3);

	cv::Rect rectangle(min.x, min.y, outputImage.size().width, outputImage.size().height);

	inputImage(rectangle).copyTo(outputImage);
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