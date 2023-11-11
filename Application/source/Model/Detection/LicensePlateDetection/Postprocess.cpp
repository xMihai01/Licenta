#include "Model/Detection/LicensePlateDetection/Postprocess.h"

bool letterLocationComparator(std::pair<cv::Mat, cv::Rect>& a, std::pair<cv::Mat, cv::Rect>& b) {
	return a.second.x < b.second.x;
}

bool letterLocationComparatorByY(std::pair<cv::Mat, cv::Rect>& a, std::pair<cv::Mat, cv::Rect>& b) {
	return a.second.y < b.second.y;
}
bool letterLocationComparatorByHeight(std::pair<cv::Mat, cv::Rect>& a, std::pair<cv::Mat, cv::Rect>& b) {
	return a.second.height < b.second.height;
}

LicensePlateDetection::Postprocess::Postprocess()
{
}

bool compareContourAreas(std::vector<cv::Point>& contour1, std::vector<cv::Point>& contour2) {
	const double i = fabs(contourArea(cv::Mat(contour1)));
	const double j = fabs(contourArea(cv::Mat(contour2)));
	return (i < j);
}

void LicensePlateDetection::Postprocess::NumberPlateExtraction(const cv::Mat& preProcessedImage, const cv::Mat& originalImage, cv::Mat& outputImage, cv::Mat& postProcessedImage)
{
	cv::Mat outImage = originalImage.clone();

	cv::Mat elementStructure = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(13, 13));
	cv::morphologyEx(preProcessedImage, postProcessedImage, cv::MORPH_OPEN, elementStructure);

	cv::subtract(preProcessedImage, postProcessedImage, postProcessedImage);

	// TODO: Histrogram for postProcessedImage
	// TODO: (DONE) Learn about normal Histogram and cumulative Histogram

	/*
	For t5.jpg, the logo was no longer detected using GaussianBlur
	For c4.jpeg, the plate couldn't be detected anymore using GaussianBlur*/
	//cv::GaussianBlur(postProcessedImage, postProcessedImage, cv::Size(5, 5), 0);

	// TODO: Learn about THRESH_TRIANGLE
	cv::threshold(postProcessedImage, postProcessedImage, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);


	std::vector<std::vector<cv::Point>> contours;
	// TODO: Replace findContours with connected components and find the highest connected component
	cv::findContours(postProcessedImage, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);
	//cv::drawContours(m_originalImage, contours, -1, cv::Scalar(0, 255, 0), 2);
	int contourNumber = 0;
	for (const auto& contour : contours) {
		CheckPlate(originalImage, outImage, contour, contourNumber);
		contourNumber++;
	}
	outputImage = outImage;
}

void LicensePlateDetection::Postprocess::NumberPlateExtractionUsingHaarCascade(const cv::Mat& preProcessedImage, const cv::Mat& originalImage, cv::Mat& outputImage)
{
	cv::Mat outImage = originalImage.clone();
	std::vector<cv::Rect> detections;
	licensePlateModel.detectMultiScale(preProcessedImage, detections, 1.1, 1);
	for (auto detection : detections) {
		cv::rectangle(outImage, detection, cv::Scalar(0, 255, 0));
	}
	outputImage = outImage;

}

void LicensePlateDetection::Postprocess::CheckPlate(const cv::Mat& originalImage, cv::Mat& outputImage, const std::vector<cv::Point>& contour, const int contourNumber)
{
	cv::RotatedRect rect = cv::minAreaRect(contour);
	if (ValidateRatio(rect)) {
		cv::Rect contourRectangle = cv::boundingRect(contour);
		double x = contourRectangle.x;
		double y = contourRectangle.y;
		double width = contourRectangle.width;
		double height = contourRectangle.height;
		cv::Mat afterValidationImage = originalImage(cv::Rect(x, y, width, height));
		if (CleanPlate(afterValidationImage)) {
			//cv::imwrite(std::to_string(contourNumber) + ".jpg", afterValidationImage);
			cv::drawContours(outputImage, std::vector<std::vector<cv::Point>>{contour}, -1, cv::Scalar(0, 255, 0), 2);
		}
	}
}

bool LicensePlateDetection::Postprocess::CleanPlate(const cv::Mat& plateImage)
{
	cv::Mat grayPlate;
	cv::cvtColor(plateImage, grayPlate, cv::COLOR_BGR2GRAY);
	cv::Mat threshPlate;

	const int BLOCK_SIZE = 11;
	cv::adaptiveThreshold(grayPlate, threshPlate, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, BLOCK_SIZE, 2);


	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(threshPlate, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

	if (contours.empty()) {
		return false;
	}

	std::vector<double> areas;
	for (const auto& contour : contours) {
		areas.push_back(cv::contourArea(contour));
	}
	int maxIndex = static_cast<int>(std::distance(areas.begin(), std::max_element(areas.begin(), areas.end())));
	std::vector<cv::Point> max_cnt = contours[maxIndex];
	double max_cntArea = areas[maxIndex];

	// Extract the bounding rectangle
	cv::Rect boundingRect = cv::boundingRect(max_cnt);
	int x = boundingRect.x;
	int y = boundingRect.y;
	int w = boundingRect.width;
	int h = boundingRect.height;

	// Extract the minimum area rectangle
	cv::RotatedRect rect = cv::minAreaRect(max_cnt);

	if (RatioCheck(max_cntArea, plateImage.size[0], plateImage.size[1], 1000, 5000)) {
		//m_postProcessedImage = plateImage;
		return true;
	}
	return false;
}

void LicensePlateDetection::Postprocess::RLSA(const cv::Mat& inputImage, cv::Mat& outputImage, const int valueHorizontal, const int valueVertical)
{
	if (valueHorizontal > 0)
		RLSAIteration(inputImage, outputImage, valueHorizontal);
	if (valueVertical > 0) {
		outputImage = inputImage.t();
		RLSAIteration(outputImage, outputImage, valueVertical);
		outputImage = outputImage.t();
	}
}

void LicensePlateDetection::Postprocess::RLSAIteration(const cv::Mat& inputImage, cv::Mat& outputImage, const int value)
{
	outputImage = inputImage.clone();
	for (int index = 0; index < inputImage.rows; index++) {

		uchar* row_pointer = outputImage.ptr<uchar>(index);
		int start = 0;

		for (int secondIndex = 0; secondIndex < inputImage.cols; secondIndex++) {

			uchar* pixel_pointer = row_pointer + secondIndex;

			if (pixel_pointer[0] == 0) {
				if (secondIndex - start <= value && secondIndex - start > 0) {
					for (int i = start; i < secondIndex; i++) {
						uchar* secondPixelPointer = row_pointer + i;
						secondPixelPointer[0] = 0;
					}
				}
				start = secondIndex;
			}
			float pixel = pixel_pointer[0];
		}
	}
}

void LicensePlateDetection::Postprocess::ClearCharImage(const cv::Mat& inputCharImage, cv::Mat& outputCharImage)
{

	std::vector<cv::Point> max_cnt;

	cv::Mat biggerInput(inputCharImage.rows + 30, inputCharImage.cols+30, CV_8UC1, cv::Scalar(255));
	inputCharImage.copyTo(biggerInput(cv::Rect(30 / 2, 30 / 2, inputCharImage.cols, inputCharImage.rows)));
	cv::Mat whiteImage(inputCharImage.rows + 30, inputCharImage.cols + 30, CV_8UC1, cv::Scalar(255));

	Utils::GetImageByHighestContour(biggerInput, outputCharImage, max_cnt);

	cv::drawContours(whiteImage, std::vector<std::vector<cv::Point>>{max_cnt}, -1, cv::Scalar(0), cv::FILLED);
	/*cv::bitwise_xor(inputCharImage, whiteImage, outputCharImage);*/
	Utils::BitwiseCharImage(biggerInput, whiteImage, outputCharImage);
}

void LicensePlateDetection::Postprocess::LetterDetection(cv::Mat& inputImage, cv::Mat& outputImage)
{
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
	cv::morphologyEx(inputImage, inputImage, cv::MORPH_OPEN, kernel);

	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(inputImage, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);

	std::vector<std::pair<cv::Mat, cv::Rect>> lettersBox;

	for (size_t index = 0; index < contours.size(); index++) {
		cv::Rect contourRectangle = cv::boundingRect(contours[index]);
		double x = contourRectangle.x;
		double y = contourRectangle.y;
		double width = contourRectangle.width; 
		double height = contourRectangle.height;
		if ((height > inputImage.rows * 0.3 && width < inputImage.cols / 2) && RatioCheck(cv::contourArea(contours[index]), 1, 1, Utils::MINIMUM_CONTOUR_AREA, Utils::MAXIMUM_CONTOUR_AREA)) {
			cv::Mat charImage = inputImage(cv::Rect(x, y, width, height));
			//ClearCharImage(inputImage(cv::Rect(x, y, width, height)), charImage);
			lettersBox.push_back(std::make_pair(charImage, contourRectangle));
		}
	}

	cv::Mat whiteImage(inputImage.rows, inputImage.cols, CV_8UC1, cv::Scalar(255));

	for (size_t index = 0; index < lettersBox.size(); index++) {
		lettersBox[index].first.copyTo(whiteImage(cv::Rect(lettersBox[index].second)));
		//cv::imwrite("chars/" + std::to_string(index) + ".jpg", lettersBox[index].first);
	}
	outputImage = whiteImage;
}

bool LicensePlateDetection::Postprocess::RatioCheck(const double area, const double width, const double height, const double ratioMin, const double ratioMax)
{

	double min = ratioMin;
	double max = ratioMax;

	double ratio = width/height;

	if (ratio < 1) {
		ratio = 1 / ratio;
	}

	/*if (ratio < ratioMin) {
		return false;
	}*/
	if ((area < min || area > max))
		return false;
	return true;
}

bool LicensePlateDetection::Postprocess::ValidateRatio(const cv::RotatedRect& rectangle)
{
	double x = rectangle.center.x;
	double y = rectangle.center.y;
	double width = rectangle.size.width;
	double height = rectangle.size.height;
	double rectangleAngle = rectangle.angle;
	double angle = 0;

	if (width > height)
		angle = -rectangleAngle;
	else
		angle = 90 + rectangleAngle;

	/*if (angle > 15) {
		return false;
	}*/

	if (height == 0 || width == 0)
		return false;

	double area = width * height;

	if (!RatioCheck(area, width, height, 1000, 5000))
		return false;
	else
		return true;

}
