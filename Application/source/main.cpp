#include <Model/VideoCamera.h>
#include <Model/Detection/LicensePlateDetection/Workflow.h>

#include <iostream>

int main() {
	
	LicensePlateDetection::Workflow licenseWorkflow;
	licenseWorkflow.DetectMultiple(LicensePlateDetection::DetectionType::DNN, "test_images.txt");

	return 0;
}