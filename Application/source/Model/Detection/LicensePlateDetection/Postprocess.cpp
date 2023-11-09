#include "Model/Detection/LicensePlateDetection/Postprocess.h"


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

void LicensePlateDetection::Postprocess::RLSA(cv::Mat& inputImage, cv::Mat& outputImage, const int valueHorizontal, const int valueVertical)
{
	if (valueHorizontal > 0)
		RLSAIteration(outputImage, outputImage, valueHorizontal);
	if (valueVertical > 0) {
		outputImage = outputImage.t();
		RLSAIteration(outputImage, outputImage, valueVertical);
		outputImage = outputImage.t();
	}
}

void LicensePlateDetection::Postprocess::RLSAIteration(cv::Mat& inputImage, cv::Mat& outputImage, const int value)
{
	for (int index = 0; index < inputImage.rows; index++) {

		uchar* row_pointer = inputImage.ptr<uchar>(index);
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
	outputImage = inputImage.clone();
}

void LicensePlateDetection::Postprocess::LetterDetection(cv::Mat& inputImage, cv::Mat& outputImage)
{
	cv::Mat original = inputImage.clone();
	//cv::Mat elementStructure = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
	//cv::morphologyEx(inputImage, outputImage, cv::MORPH_CLOSE, elementStructure);
	//
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(inputImage, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);

	cv::Mat contouredImage = inputImage.clone();
	cv::cvtColor(contouredImage, contouredImage, cv::COLOR_GRAY2BGR);
	cv::drawContours(contouredImage, contours, -1, cv::Scalar(0, 255, 0), 1);
	std::vector<std::pair<cv::Mat, cv::Rect>> lettersBox;


	for (size_t index = 0; index < contours.size(); index++) {
		cv::Rect contourRectangle = cv::boundingRect(contours[index]);
		double x = contourRectangle.x;
		double y = contourRectangle.y;
		double width = contourRectangle.width;
		double height = contourRectangle.height;
		//if (m_postprocessing.RatioCheck(cv::contourArea(contours[index]), 1, 1, 50, 10000)) {
		if (height > outputImage.cols * 0.075 && width < outputImage.rows / 2) {
			cv::Mat charImage = outputImage(cv::Rect(x, y, width, height));
			lettersBox.push_back(std::make_pair(charImage, contourRectangle));
		}
	}

	//std::sort(lettersBox.begin(), lettersBox.end(), letterLocationComparator);
	//std::sort(lettersBox.begin(), lettersBox.end(), letterLocationComparatorByY);

	double startY = inputImage.rows - 1;
	double endY = 1;
	for (auto& letter : lettersBox) {
		if (letter.second.height > endY)
			endY = letter.second.height;
		if (letter.second.y < startY)
			startY = letter.second.y;
	}


	cv::Mat whiteImage(100, 300, CV_8UC1, cv::Scalar(255));
	cv::Mat croppedImage = outputImage(cv::Rect(0, startY, inputImage.cols, endY));
	croppedImage.copyTo(whiteImage(cv::Rect(0, startY, inputImage.cols, endY)));

	outputImage = whiteImage;

	std::vector<std::vector<cv::Point>> contours2;
	cv::findContours(outputImage, contours2, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);

	cv::cvtColor(outputImage, outputImage, cv::COLOR_GRAY2BGR);
	cv::drawContours(outputImage, contours2, -1, cv::Scalar(0, 255, 0), 1);
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
