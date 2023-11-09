#include "Model/Detection/LicensePlateDetection/Workflow.h"

bool letterLocationComparator(std::pair<cv::Mat, cv::Rect>& a, std::pair<cv::Mat, cv::Rect>& b) {
	return a.second.x < b.second.x;
}

bool letterLocationComparatorByY(std::pair<cv::Mat, cv::Rect>& a, std::pair<cv::Mat, cv::Rect>& b) {
	return a.second.y < b.second.y;
}
bool letterLocationComparatorByHeight(std::pair<cv::Mat, cv::Rect>& a, std::pair<cv::Mat, cv::Rect>& b) {
	return a.second.height < b.second.height;
}

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
		//m_preprocessing.NoiseReduction(outputImage, outputImage, Gaussian);
		m_detector.IsModelReady() ? m_preprocessing.SkewCorrection(outputImage, originalInputImage) : outputImage = originalInputImage;
		outputImage = originalInputImage;
		//m_detector.IsModelReady() ? RLSA(outputImage, originalInputImage, 10, 10) : outputImage = originalInputImage;
		//outputImage = originalInputImage;
		m_detector.IsModelReady() ? m_postprocessing.LetterDetection(outputImage, originalInputImage) : outputImage = originalInputImage;
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

		cv::imwrite(std::to_string(i) + imageName, outputImage);
	}
}

