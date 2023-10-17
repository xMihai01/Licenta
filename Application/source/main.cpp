#include <Model/VideoCamera.h>
#include <Model/Detection/LicensePlateDetection/Preprocess.h>
#include <Model/Detection/LicensePlateDetection/Postprocess.h>

#include <iostream>

int main() {

	//VideoCamera videoCamera;
	//videoCamera.OpenCamera();
	//std::cout << "\n\n" << videoCamera.GetFrameHeight() << " " << videoCamera.GetFrameWidth();
	//while (videoCamera.IsCameraOpened()) {
	//	cv::imshow("Frame", videoCamera.GetCurrentFrame());
	//	if (cv::waitKey(30) == 27)
	//		break;
	//}
	//videoCamera.StopCamera();
	cv::Mat testImage = cv::imread("C:/Users/mihai/Desktop/Licenta/Licenta/Resources/t2.jpg");
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
    cv::imshow("final", detectedImage);
	cv::imshow("window", postProcessedImage);
	cv::imshow("win", testImage);

	while (true) {
		cv::waitKey(30);
	}
	
	return 0;
}