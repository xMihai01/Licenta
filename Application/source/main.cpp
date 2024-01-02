#include <View/mainwindow.h>

int main(int argc, char* argv[]) {
	
	//LicensePlateDetection::Workflow licenseWorkflow;
	//cv::Mat testImage = cv::imread("C:/Users/mihai/Desktop/Licenta/Licenta/Resources/t2.jpg"); //t2.jpg 7-V, //n8.JPEG 0-D, 20 21 27
	//licenseWorkflow.Detect(testImage, testImage, std::string(), LicensePlateDetection::DetectionType::DNN);
	//licenseWorkflow.DetectMultiple(LicensePlateDetection::DetectionType::DNN, "test_images.txt");
	
	QApplication application(argc, argv);
	MainWindow window;
	window.show();
	return application.exec();
}