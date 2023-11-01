#include "Model/Detection/LicensePlateDetection/Workflow.h"

LicensePlateDetection::Workflow::Workflow()
{
	m_detector.ChangeDetectionModel("license_weights.onnx", "license_classList.txt", 480.0, 480.0, 0.4);
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
	for (std::string imageName : imageNames) {
		std::cout << "\nCurrent image: " + imageName;
		cv::Mat testImage = cv::imread("../../Licenta/Resources/" + imageName);
		cv::Mat outputImage;
		licenseWorkflow.Detect(testImage, outputImage, std::string(), detectionType);

		cv::imwrite(imageName + "_1.jpg", outputImage);
	}
}
