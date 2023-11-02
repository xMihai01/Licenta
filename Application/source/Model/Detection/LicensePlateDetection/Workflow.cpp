#include "Model/Detection/LicensePlateDetection/Workflow.h"

LicensePlateDetection::Workflow::Workflow()
{
	m_detector.ChangeDetectionModel("license_weights_480trained.onnx", "license_classList.txt", 480.0, 480.0, 0.4);
}

void LicensePlateDetection::Workflow::Detect(cv::Mat& inputImage, cv::Mat& outputImage, std::string& outputText, const LicensePlateDetection::DetectionType detectionType)
{
	cv::Mat originalInputImage;
	switch (detectionType)
	{
	case DetectionType::IMAGE_PROCESSING:
		m_preprocessing.ResizeImage(inputImage, inputImage, 640, 480);
		m_preprocessing.ConvertImageToGray(inputImage, outputImage);
		m_preprocessing.NoiseReduction(outputImage, outputImage, BilateralFilter);
		//m_preprocessing.AdaptiveHistogramEqualization(outputImage, outputImage);

		originalInputImage = inputImage.clone();
		m_postprocessing.NumberPlateExtraction(outputImage, originalInputImage, outputImage);

		break;
	case DetectionType::HAAR_CASCADE:
		m_preprocessing.ResizeImage(inputImage, inputImage, 640, 480);
		m_preprocessing.PreProcessForHaarCascade(inputImage, outputImage);

		originalInputImage = inputImage.clone();
		m_postprocessing.NumberPlateExtractionUsingHaarCascade(outputImage, originalInputImage, outputImage);

		break;
	case DetectionType::DNN:
		m_detector.IsModelReady() ? outputImage = m_detector.Detect(inputImage) : outputImage = originalInputImage;
		m_detector.IsModelReady() ? SkewCorrection(outputImage, originalInputImage) : outputImage = originalInputImage;
		outputImage = originalInputImage;
		break;
	default:
		break;
	}
	// TODO: change outputText after implementing text recognition
}

void LicensePlateDetection::Workflow::DetectMultiple(const LicensePlateDetection::DetectionType detectionType, const std::string& fileName)
{
	std::vector<std::string> imageNames = Utils::GetImageNamesFromFile("../Resources/" + fileName);

	LicensePlateDetection::Workflow licenseWorkflow;
	int i = 0;
	for (std::string imageName : imageNames) {
		i++;
		std::cout << "\nCurrent image: " + imageName;
		cv::Mat testImage = cv::imread("../../Licenta/Resources/" + imageName);
		cv::Mat outputImage;
		licenseWorkflow.Detect(testImage, outputImage, std::string(), detectionType);

		cv::imwrite(std::to_string(i) + "_" + imageName + ".jpg", outputImage);
	}
}

void LicensePlateDetection::Workflow::SkewCorrection(cv::Mat& inputImage, cv::Mat& outputImage)
{
	m_preprocessing.ConvertImageToGray(inputImage, outputImage);

	cv::threshold(outputImage, outputImage, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);

	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(outputImage, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

	std::vector<double> areas;
	for (const auto& contour : contours) {
		areas.push_back(cv::contourArea(contour));
	}

	int maxIndex = static_cast<int>(std::distance(areas.begin(), std::max_element(areas.begin(), areas.end())));
	std::vector<cv::Point> max_cnt = contours[maxIndex];

	cv::Rect contourRectangle = cv::boundingRect(max_cnt);
	double x = contourRectangle.x;
	double y = contourRectangle.y;
	double width = contourRectangle.width;
	double height = contourRectangle.height;
	outputImage = inputImage(cv::Rect(x, y, width, height));

	m_preprocessing.ConvertImageToGray(outputImage, outputImage);

	cv::bitwise_not(outputImage, outputImage);
	cv::threshold(outputImage, outputImage, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);

	cv::RotatedRect rotatedRect = cv::minAreaRect(max_cnt);
	float angle = rotatedRect.angle;

	if (angle > 80 && angle < 10) // image doesn't need skew correction
		return;

	int h = outputImage.rows;
	int w = outputImage.cols;
	cv::Point2f center(w / 2, h / 2);
	cv::Mat M = cv::getRotationMatrix2D(center, angle < 45 ? angle : angle-90, 1.0); // bottom-left down - 90. bottom-left up + 0

	cv::warpAffine(outputImage, outputImage, M, cv::Size(w, h), cv::INTER_CUBIC, cv::BORDER_REPLICATE);
	
}
