#include "Model/Detection/LicensePlateDetection/Workflow.h"

LicensePlateDetection::Workflow::Workflow()
{
	//m_detector.ChangeDetectionModel("license_weights_640trained.onnx", "license_classList.txt", 640.0, 640.0, 0.4);
	m_detector.ChangeDetectionModel(Utils::PLATE_MODEL_NAME, Utils::PLATE_MODEL_CLASS_LIST, Utils::INPUT_WIDTH, Utils::INPUT_HEIGHT, 0.4);
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
		m_preprocessing.Undistortion(outputImage, outputImage);
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
		m_preprocessing.Undistortion(outputImage, outputImage);
		//m_detector.IsModelReady() ? m_preprocessing.SkewCorrection(m_detector.Detect(inputImage), originalInputImage) : outputImage = originalInputImage;
		//outputImage = originalInputImage;
		//m_postprocessing.CleanPlateDetection(outputImage, originalInputImage);
		//outputImage = originalInputImage;
	/*	m_detector.IsModelReady() ? m_postprocessing.LetterDetection(outputImage, originalInputImage) : outputImage = originalInputImage;
		outputImage = originalInputImage;*/
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

		//std::vector<std::vector<cv::Point>> contours;
		//cv::findContours(outputImage, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);
		//cv::cvtColor(outputImage, outputImage, cv::COLOR_GRAY2BGR);
		//cv::drawContours(outputImage, contours, -1, cv::Scalar(0, 255, 0), 1);

		cv::imwrite("../../test/" + std::to_string(i) + "_" + imageName, outputImage);
		//system("pause");
	}
	
}

