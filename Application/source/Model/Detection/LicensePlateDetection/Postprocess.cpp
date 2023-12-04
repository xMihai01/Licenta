#include "Model/Detection/LicensePlateDetection/Postprocess.h"

bool letterLocationComparator(std::pair<cv::Mat, cv::Rect>& a, std::pair<cv::Mat, cv::Rect>& b) {
	return a.second.x < b.second.x;
}

LicensePlateDetection::Postprocess::Postprocess()
{
}

void LicensePlateDetection::Postprocess::NumberPlateExtractionUsingImageProcessing(const cv::Mat& preProcessedImage, const cv::Mat& originalImage, cv::Mat& detectedPlate)
{
	cv::Mat outImage = originalImage.clone();
	cv::Mat postProcessedImage;

	cv::Mat elementStructure = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(13, 13));
	cv::morphologyEx(preProcessedImage, postProcessedImage, cv::MORPH_OPEN, elementStructure);

	cv::subtract(preProcessedImage, postProcessedImage, postProcessedImage);

	cv::threshold(postProcessedImage, postProcessedImage, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);

	std::vector<std::vector<cv::Point>> contours;
	std::vector<std::vector<cv::Point>> validContours;

	cv::findContours(postProcessedImage, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);
	//cv::drawContours(outImage, contours, -1, cv::Scalar(0, 255, 0), 2);
	int contourNumber = 0;
	for (const auto& contour : contours) {
		if (CheckPlate(originalImage, outImage, contour, contourNumber)) {
			validContours.push_back(contour);
		}
		contourNumber++;
	}

	// TODO: Throw an exception
	if (!validContours.size()) {
		detectedPlate = outImage;
		return;
	}

	std::vector<cv::Point> max_cnt;
	Utils::GetImageByHighestContour(cv::Mat(), cv::Mat(), max_cnt, false, validContours);
	cv::Rect contourRectangle = cv::boundingRect(max_cnt);

	detectedPlate = outImage(cv::Rect(contourRectangle.x, contourRectangle.y, contourRectangle.width, contourRectangle.height));
	//cv::drawContours(outImage, std::vector<std::vector<cv::Point>>{max_cnt}, -1, cv::Scalar(255, 0, 0), 2);
}

void LicensePlateDetection::Postprocess::NumberPlateExtractionUsingHaarCascade(const cv::Mat& preProcessedImage, const cv::Mat& originalImage, cv::Mat& detectedPlate)
{
	cv::Mat outImage = originalImage.clone();
	std::vector<cv::Rect> detections;
	licensePlateModel.detectMultiScale(preProcessedImage, detections, 1.1, 1);
	for (auto detection : detections) {
		cv::rectangle(outImage, detection, cv::Scalar(0, 255, 0));
	}

	detectedPlate = outImage;
}

void LicensePlateDetection::Postprocess::NumberPlateExtractionUsingDNN(const cv::Mat& originalImage, cv::Mat& detectedPlate, ObjectDetector* detector)
{
	if (detector->IsModelReady())
		detectedPlate = detector->Detect(originalImage);
}

bool LicensePlateDetection::Postprocess::CheckPlate(const cv::Mat& originalImage, cv::Mat& outputImage, const std::vector<cv::Point>& contour, const int contourNumber)
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
			//cv::drawContours(outputImage, std::vector<std::vector<cv::Point>>{contour}, -1, cv::Scalar(0, 255, 0), 2);
			return true;
		}
	}
	return false;
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

	if (RatioCheck(max_cntArea, plateImage.size[0], plateImage.size[1], 1000, 4000)) {
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
	cv::Mat invertedChar = inputCharImage.clone();
	cv::threshold(invertedChar, invertedChar, 0, 255, cv::THRESH_BINARY);
	cv::bitwise_not(invertedChar, invertedChar);
	cv::Mat biggerInput(inputCharImage.rows + 30, inputCharImage.cols+30, CV_8UC1, cv::Scalar(0));
	invertedChar.copyTo(biggerInput(cv::Rect(30 / 2, 30 / 2, inputCharImage.cols, inputCharImage.rows)));
	cv::Mat blackImage(inputCharImage.rows + 30, inputCharImage.cols + 30, CV_8UC1, cv::Scalar(0));

	Utils::GetImageByHighestContour(biggerInput, outputCharImage, max_cnt);

	//cv::cvtColor(biggerInput, biggerInput, cv::COLOR_GRAY2BGR);
	cv::drawContours(blackImage, std::vector<std::vector<cv::Point>>{max_cnt}, -1, cv::Scalar(255), cv::FILLED);

	Utils::BitwiseCharImage(biggerInput, blackImage, outputCharImage);
	
	cv::RotatedRect rotatedRect = cv::minAreaRect(max_cnt);
	float angle = rotatedRect.angle;

	if (angle > 88 && angle < 2) // image doesn't need skew correction
		return;

	int h = biggerInput.rows;
	int w = biggerInput.cols;
	cv::Point2f center(w / 2, h / 2);
	cv::Mat rotationMatrix = cv::getRotationMatrix2D(center, angle < 45 ? angle : angle - 90, 1.0); // bottom-left down - 90. bottom-left up + 0

	cv::warpAffine(outputCharImage, outputCharImage, rotationMatrix, cv::Size(w, h), cv::INTER_CUBIC, cv::BORDER_REPLICATE);
	cv::bitwise_not(outputCharImage, outputCharImage);

}

char LicensePlateDetection::Postprocess::RecognizeCharacterUsingTemplateMatching(const cv::Mat& inputCharImage)
{
	cv::Mat charImage = inputCharImage.clone();

	std::vector<std::string> imageNames = Utils::GetImageNamesFromFile("../Resources/charTemplates.txt");
	double threshold = 0.40;
	std::pair<double, char> maxChar = std::make_pair(0, '-');

	for (std::string imageName : imageNames) {
		//std::cout << "\nCurrent image: " + imageName;
		cv::Mat templateImage = cv::imread("../../Licenta/Resources/CharTemplates/" + imageName, cv::IMREAD_GRAYSCALE);
		cv::resize(charImage, charImage, cv::Size(templateImage.cols, templateImage.rows));
		cv::Mat result;
		cv::matchTemplate(charImage, templateImage, result, cv::TM_CCOEFF_NORMED);

		double minVal, maxVal;
		cv::Point minLoc, maxLoc;
		cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);
		//std::cout << "\nMAX VAL: " << maxVal << "C: " << imageName[0] << "\n";
		if (maxVal > maxChar.first && maxVal > threshold)
			maxChar = std::make_pair(maxVal, imageName[0]);
		
	}
	//std::cout << "\nC: " << maxChar.second << "\n";
	return maxChar.second;

}


void LicensePlateDetection::Postprocess::LetterDetection(cv::Mat& inputImage, cv::Mat& outputImage, std::string& detectedText)
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
		if ((height > inputImage.rows * 0.2 && width < inputImage.cols / 2) && RatioCheck(cv::contourArea(contours[index]), 1, 1, Utils::MINIMUM_CONTOUR_AREA, Utils::MAXIMUM_CONTOUR_AREA)) {
			cv::Mat charImage = inputImage(cv::Rect(x, y, width, height));
			//ClearCharImage(inputImage(cv::Rect(x, y, width, height)), charImage);
			lettersBox.push_back(std::make_pair(charImage, contourRectangle));
		}
	}

	cv::Mat whiteImage(inputImage.rows, inputImage.cols, CV_8UC1, cv::Scalar(255));
	std::sort(lettersBox.begin(), lettersBox.end(), Utils::letterLocationComparator);
	std::string text = "";
	for (size_t index = 0; index < lettersBox.size(); index++) {
		lettersBox[index].first.copyTo(whiteImage(cv::Rect(lettersBox[index].second)));
		cv::Mat clearChar;
		//ClearCharImage(lettersBox[index].first, clearChar);
		char ch = RecognizeCharacterUsingTemplateMatching(lettersBox[index].first);
		if (ch != '-')
			text += ch;
		//ClearCharImage(lettersBox[index].first, clearChar);
		//cv::resize(clearChar, clearChar, cv::Size(lettersBox[index].first.cols, lettersBox[index].first.rows));
		//clearChar.copyTo(whiteImage(cv::Rect(lettersBox[index].second)));
		//cv::imwrite("chars/" + std::to_string(index) + ".jpg", clearChar);
	}
	detectedText = text;
	outputImage = whiteImage;
}

void LicensePlateDetection::Postprocess::CleanPlateDetection(cv::Mat& inputImage, cv::Mat& outputImage)
{
	cv::Mat labels, stats, centroids;
	outputImage = inputImage.clone();
	int numComponents = cv::connectedComponentsWithStats(inputImage, labels, stats, centroids);
	cv::cvtColor(outputImage, outputImage, cv::COLOR_GRAY2BGR);
	// Iterate through components and draw rectangles
	int maxAreaIndex = -1;
	int maxArea = -1;

	std::vector<cv::Vec3b> colors(numComponents);

	for (int i = 0; i < numComponents; i++) {
		colors[i] = cv::Vec3b(rand() % 255, rand() % 255, rand() % 255);
	}

	for (int i = 0; i < outputImage.cols; i++) {
		for (int j = 0; j < outputImage.rows; j++) {
			if (labels.at<int>(cv::Point(i, j)) != 0) {
				outputImage.at<cv::Vec3b>(cv::Point(i, j)) = colors[(int)labels.at<int>(cv::Point(i, j))];
			}
		}
	}

	//for (int i = 1; i < numComponents; i++) {
	//	int area = stats.at<int>(i, cv::CC_STAT_AREA);

	//	if (area > maxArea) {
	//		maxArea = area;
	//		maxAreaIndex = i;
	//	}
	//}


	//// Draw a rectangle around the component with the second-highest area
	//if (maxAreaIndex != -1) {
	//	cv::Rect boundingBox(stats.at<int>(maxAreaIndex, cv::CC_STAT_LEFT),
	//		stats.at<int>(maxAreaIndex, cv::CC_STAT_TOP),
	//		stats.at<int>(maxAreaIndex, cv::CC_STAT_WIDTH),
	//		stats.at<int>(maxAreaIndex, cv::CC_STAT_HEIGHT));

	//	cv::rectangle(outputImage, boundingBox, cv::Scalar(0, 255, 0), 2);
	//}
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
