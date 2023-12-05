#include "Model/Detection/LicensePlateDetection/Workflow.h"

LicensePlateDetection::Workflow::Workflow()
{
	m_detector = new ObjectDetector;
	m_detector->ChangeDetectionModel(Utils::PLATE_MODEL_NAME, Utils::PLATE_MODEL_CLASS_LIST, Utils::INPUT_WIDTH, Utils::INPUT_HEIGHT, 0.4);
}

void LicensePlateDetection::Workflow::Detect(cv::Mat& inputImage, cv::Mat& outputImage, std::string& outputText, const LicensePlateDetection::DetectionType detectionType)
{
	cv::Mat originalInputImage;
	switch (detectionType)
	{
	case DetectionType::IMAGE_PROCESSING:
		m_preprocessing.ResizeImage(inputImage, inputImage, 640, 480);
		originalInputImage = inputImage.clone();
		m_preprocessing.ConvertImageToGray(inputImage, outputImage);
		m_preprocessing.NoiseReduction(outputImage, outputImage, BilateralFilter);

		m_postprocessing.NumberPlateExtractionUsingImageProcessing(outputImage, originalInputImage, outputImage);
		break;
	case DetectionType::HAAR_CASCADE:
		m_preprocessing.ResizeImage(inputImage, inputImage, 640, 480);
		m_preprocessing.PreProcessForHaarCascade(inputImage, outputImage);

		originalInputImage = inputImage.clone();
		m_postprocessing.NumberPlateExtractionUsingHaarCascade(outputImage, originalInputImage, outputImage);

		break;
	case DetectionType::DNN:
		m_postprocessing.NumberPlateExtractionUsingDNN(inputImage, outputImage, this->m_detector);
		break;
	default:
		break;
	}
	m_preprocessing.Undistortion(outputImage, outputImage);
	//m_preprocessing.SkewCorrection(outputImage, outputImage);
	m_postprocessing.LetterDetection(outputImage, outputImage, outputText);
	//std::cout << "\nTEXT: " << outputText << "\n";
}

void LicensePlateDetection::Workflow::DetectMultiple(const LicensePlateDetection::DetectionType detectionType, const std::string& fileName)
{
	std::vector<std::string> imageNames = Utils::GetImageNamesFromFile("../Resources/" + fileName);
	std::vector<std::string> plateWords = Utils::GetImageNamesFromFile("../Resources/expectedPlates.txt");

	LicensePlateDetection::Workflow licenseWorkflow;
	
	int numberOfPlates = plateWords.size();
	int correctPlates = 0;

	int i = 0;
	for (std::string imageName : imageNames) {
		i++;
		std::cout << "\nCurrent image: " + imageName;
		cv::Mat testImage = cv::imread("../../Licenta/Resources/" + imageName);
		cv::Mat outputImage;
		std::string text;
		licenseWorkflow.Detect(testImage, outputImage, text, detectionType);
		if (text == plateWords[i - 1])
			correctPlates++;
		//std::vector<std::vector<cv::Point>> contours;
		//cv::findContours(outputImage, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);
		//cv::cvtColor(outputImage, outputImage, cv::COLOR_GRAY2BGR);
		//cv::drawContours(outputImage, contours, -1, cv::Scalar(0, 255, 0), 1);

		cv::imwrite("../../test/" + std::to_string(i) + "_" + imageName, outputImage);
		//system("pause");
		std::cout << "\nTEXT: " << text << " expected: " << plateWords[i-1] << "\n";
	}
	std::cout << "Accuraccy: " << (float)correctPlates / (float)numberOfPlates * 100.0f << "%";
}

