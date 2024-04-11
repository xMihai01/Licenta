#include <View/mainwindow.h>
#include <Model/Database/BusinessLogic/Session.h>

int main(int argc, char* argv[]) {
	
	//std::string test = "";
	//LicensePlateDetection::Workflow licenseWorkflow;
	//cv::Mat testImage = cv::imread("C:/Users/mihai/Desktop/Licenta/Licenta/Resources/n19.JPEG"); //t2.jpg 7-V, //n8.JPEG 0-D, 20 21 2
	//cv::Mat testImage = cv::imread("C:/Users/mihai/Desktop/Products/parking_spaces/8.jpg");
	//cv::Mat testImage = cv::imread("C:/Users/mihai/Desktop/Products/testImageLicenta.jpeg");
	//licenseWorkflow.Detect(testImage, testImage, test, LicensePlateDetection::DetectionType::IMAGE_PROCESSING);
	//std::cout << "Test: " << test;
	//licenseWorkflow.DetectMultiple(LicensePlateDetection::DetectionType::DNN, "test_images.txt");
	
	QApplication application(argc, argv);
	MainWindow window;
	window.show();
	return application.exec();

	//cv::imwrite("C:\\Users\\mihai\\Desktop\\Licenta\\Build_Licenta\\Application\\pozeLic\\averageFilter.jpg", filter1);
	//cv::imwrite("C:\\Users\\mihai\\Desktop\\Licenta\\Build_Licenta\\Application\\pozeLic\\gaussianFilter.jpg", filter2);
	//cv::imwrite("C:\\Users\\mihai\\Desktop\\Licenta\\Build_Licenta\\Application\\pozeLic\\bilateralFilter.jpg", filter3);
	//cv::imwrite("C:\\Users\\mihai\\Desktop\\Licenta\\Build_Licenta\\Application\\pozeLic\\medianFilter.jpg", filter4);
}