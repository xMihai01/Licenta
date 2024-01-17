#include <View/mainwindow.h>
#include <Model/Database/BusinessLogic/Session.h>

int main(int argc, char* argv[]) {
	
	std::string test = "";
	//LicensePlateDetection::Workflow licenseWorkflow;
	//cv::Mat testImage = cv::imread("C:/Users/mihai/Desktop/Licenta/Licenta/Resources/t2.jpg"); //t2.jpg 7-V, //n8.JPEG 0-D, 20 21 27
	//cv::Mat testImage = cv::imread("C:/Users/mihai/Desktop/Products/img/original_11.jpg");
	//licenseWorkflow.Detect(testImage, testImage, test, LicensePlateDetection::DetectionType::DNN);
	//std::cout << "Test: " << test;
	//licenseWorkflow.DetectMultiple(LicensePlateDetection::DetectionType::DNN, "test_images.txt");
	
	QApplication application(argc, argv);
	MainWindow window;
	window.show();
	return application.exec();
}