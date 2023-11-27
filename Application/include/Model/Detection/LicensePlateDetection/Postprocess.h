#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>

#include <Model/Utils.h>
#include <Model/Detection/ObjectDetector.h>

namespace LicensePlateDetection {

	class Postprocess {
	private:

		inline static cv::CascadeClassifier licensePlateModel = cv::CascadeClassifier("C:/Users/mihai/Desktop/Licenta/Licenta/Resources/license.xml");

	public:

		Postprocess();

		void NumberPlateExtractionUsingImageProcessing(const cv::Mat& preProcessedImage, const cv::Mat& originalImage, cv::Mat& detectedPlate);
		void NumberPlateExtractionUsingHaarCascade(const cv::Mat& preProcessedImage, const cv::Mat& originalImage, cv::Mat& detectedPlate);
		void NumberPlateExtractionUsingDNN(const cv::Mat& originalImage, cv::Mat& detectedPlate, ObjectDetector* detector);

		void LetterDetection(cv::Mat& inputImage, cv::Mat& outputImage);
		void CleanPlateDetection(cv::Mat& inputImage, cv::Mat& outputImage);

	private:

		bool CheckPlate(const cv::Mat& originalImage, cv::Mat& outputImage, const std::vector<cv::Point>& contour, const int contourNumber);
		bool CleanPlate(const cv::Mat& plateImage);
		bool ValidateRatio(const cv::RotatedRect& rectangle);
		bool RatioCheck(const double area, const double width, const double height, const double ratioMin, const double ratioMax);

		void RLSA(const cv::Mat& inputImage, cv::Mat& outputImage, const int valueHorizontal = -1, const int valueVertical = -1);
		void RLSAIteration(const cv::Mat& inputImage, cv::Mat& outputImage, const int value);

		void ClearCharImage(const cv::Mat& inputCharImage, cv::Mat& outputCharImage);
	};

}