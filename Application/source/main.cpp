#include <Model/VideoCamera.h>
#include <Model/Detection/LicensePlateDetection/Preprocess.h>
#include <Model/Detection/LicensePlateDetection/Postprocess.h>
#include <Model/Utils.h>

#include <iostream>

int main() {

	std::vector<std::string> imageNames = Utils::GetImageNamesFromFile("../Resources/test_images.txt");

	//cv::Mat testImage = cv::imread("C:/Users/mihai/Desktop/Licenta/Licenta/Resources/BV20WXP.jpeg");
	for (std::string imageName : imageNames) {
		std::cout << "\nCurrent image: " + imageName;
		cv::Mat testImage = cv::imread("../../Licenta/Resources/" + imageName);
		LicensePlateDetection::Preprocess preprocessTest;
		preprocessTest.ResizeImage(testImage, testImage, 640, 480);
		cv::Mat originalTestImage = testImage.clone();
		preprocessTest.ConvertImageToGray(testImage, testImage);
		preprocessTest.NoiseReduction(testImage, testImage, SmoothingAlgorithm::BilateralFilter);

		cv::Mat postProcessedImage;
		cv::Mat detectedImage;
		//preprocessTest.AdaptiveHistogramEqualization(testImage, testImage);

		LicensePlateDetection::Postprocess postprocessTest;
		postprocessTest.NumberPlateExtraction(testImage, originalTestImage, postProcessedImage, detectedImage);

		cv::imwrite(imageName + "_histogram.jpg", Utils::GetHistograms(testImage).first);
	}
	//postprocessTest.NumberPlateExtractionUsingHaarCascade(testImage, originalTestImage, detectedImage);
    
	/*cv::imshow("final", detectedImage);
	cv::imshow("window", postProcessedImage);
	cv::imshow("histogram", Utils::GetHistograms(testImage).first);
	cv::imshow("win", testImage);*/

	/*while (true) {
		cv::waitKey(30);
	}*/
	
	return 0;
}