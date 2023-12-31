#include "View/mainwindow.h"
#include <Model/VideoCamera.h>
#include <Model/Detection/LicensePlateDetection/Workflow.h>

#include <iostream>
#include <QApplication>

int main(int argc, char* argv[]) {
	
	LicensePlateDetection::Workflow licenseWorkflow;
	//cv::Mat testImage = cv::imread("C:/Users/mihai/Desktop/Licenta/Licenta/Resources/t2.jpg"); //t2.jpg 7-V, //n8.JPEG 0-D, 20 21 27
	//licenseWorkflow.Detect(testImage, testImage, std::string(), LicensePlateDetection::DetectionType::DNN);
	//licenseWorkflow.DetectMultiple(LicensePlateDetection::DetectionType::DNN, "test_images.txt");
	
	QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

	//return 0;
}