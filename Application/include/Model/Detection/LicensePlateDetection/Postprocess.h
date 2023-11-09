#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>

namespace LicensePlateDetection {

	class Postprocess {
	private:

		inline static cv::CascadeClassifier licensePlateModel = cv::CascadeClassifier("C:/Users/mihai/Desktop/Licenta/Licenta/Resources/license.xml");

	public:

		Postprocess();

		void NumberPlateExtraction(const cv::Mat& preProcessedImage, const cv::Mat& originalImage, cv::Mat& outputImage, cv::Mat& postProcessedImage = cv::Mat());

		void NumberPlateExtractionUsingHaarCascade(const cv::Mat& preProcessedImage, const cv::Mat& originalImage, cv::Mat& outputImage);

		void LetterDetection(cv::Mat& inputImage, cv::Mat& outputImage);

	private:

		void CheckPlate(const cv::Mat& originalImage, cv::Mat& outputImage, const std::vector<cv::Point>& contour, const int contourNumber);
		bool CleanPlate(const cv::Mat& plateImage);
		bool ValidateRatio(const cv::RotatedRect& rectangle);
		bool RatioCheck(const double area, const double width, const double height, const double ratioMin, const double ratioMax);

		void RLSA(cv::Mat& inputImage, cv::Mat& outputImage, const int valueHorizontal = -1, const int valueVertical = -1);
		void RLSAIteration(cv::Mat& inputImage, cv::Mat& outputImage, const int value);

	};

}