#include <Model/VideoCamera.h>
#include <Model/Detection/LicensePlateDetection/Preprocess.h>
#include <Model/Detection/LicensePlateDetection/Postprocess.h>
#include <Model/Detection/ObjectDetector.h>
#include <Model/Utils.h>

#include <iostream>

int main() {
	
	std::vector<std::string> imageNames = Utils::GetImageNamesFromFile("../Resources/test_images.txt");
	LicensePlateDetection::Preprocess preprocessTest;
	ObjectDetector detector;
	try {
		detector.ChangeDetectionModel("car_weights.onnx", "car_classList.txt", 640, 480, 0.6);
	}
	catch (std::exception ex) {
		std::cout <<  ex.what();
	}

	//cv::Mat testImage = cv::imread("C:/Users/mihai/Desktop/Licenta/Licenta/Resources/BV20WXP.jpeg");
	for (std::string imageName : imageNames) {
		std::cout << "\nCurrent image: " + imageName;
		cv::Mat testImage = cv::imread("../../Licenta/Resources/" + imageName);

		preprocessTest.ResizeImage(testImage, testImage, 640, 480);

		try {
			testImage = detector.Detect(testImage);
		}
		catch (std::exception ex) {
			std::cout << ex.what();
			continue;
		}

		cv::Mat originalTestImage = testImage.clone();
		preprocessTest.ConvertImageToGray(testImage, testImage);
		preprocessTest.NoiseReduction(testImage, testImage, SmoothingAlgorithm::BilateralFilter);
		//preprocessTest.PreProcessForHaarCascade(testImage, testImage);

		cv::Mat postProcessedImage;
		cv::Mat detectedImage;
		//preprocessTest.AdaptiveHistogramEqualization(testImage, testImage);

		LicensePlateDetection::Postprocess postprocessTest;
		//postprocessTest.NumberPlateExtractionUsingHaarCascade(testImage, originalTestImage, detectedImage);
		postprocessTest.NumberPlateExtraction(testImage, originalTestImage, postProcessedImage, detectedImage);

		//cv::imwrite(imageName + "_detectedCar_yolo_training1.jpg", detectedImage);
		cv::imwrite(imageName + "_detectedLicense_yolo_1.jpg", detectedImage);
	}

	//cv::imshow("final", detectedImage);
	//cv::imshow("window", postProcessedImage);
	//cv::imshow("histogram", Utils::GetHistograms(testImage).first);
	//cv::imshow("win", testImage);

	while (true) {
		cv::waitKey(30);
	}

	return 0;
}