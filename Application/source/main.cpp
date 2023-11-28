#include <Model/VideoCamera.h>
#include <Model/Detection/LicensePlateDetection/Workflow.h>

#include <iostream>

int main() {
	
	LicensePlateDetection::Workflow licenseWorkflow;
	//cv::Mat testImage = cv::imread("C:/Users/mihai/Desktop/Licenta/Licenta/Resources/tcar1.jpg");
	//licenseWorkflow.Detect(testImage, testImage, std::string(), LicensePlateDetection::DetectionType::IMAGE_PROCESSING);
	licenseWorkflow.DetectMultiple(LicensePlateDetection::DetectionType::DNN, "test_images.txt");
	
	return 0;
}