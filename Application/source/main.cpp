#include <Model/VideoCamera.h>
#include <Model/Detection/LicensePlateDetection/Workflow.h>

#include <iostream>

int main() {
	
	LicensePlateDetection::Workflow licenseWorkflow;
	//cv::Mat testImage = cv::imread("C:/Users/mihai/Desktop/Licenta/Licenta/Resources/n9.JPEG"); // test on BV20WXP.jpeg
	//licenseWorkflow.Detect(testImage, testImage, std::string(), LicensePlateDetection::DetectionType::DNN);
	licenseWorkflow.DetectMultiple(LicensePlateDetection::DetectionType::IMAGE_PROCESSING, "test_images.txt");

	return 0;
}