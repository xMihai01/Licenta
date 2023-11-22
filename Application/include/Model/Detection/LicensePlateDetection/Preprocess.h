#include <opencv2/opencv.hpp>

#include "SmoothingAlgorithmEnum.h"
#include <Model/Utils.h>

namespace LicensePlateDetection {

	class Preprocess {

	private:

		const std::uint8_t DIAMETER = 5;
		const std::uint16_t SIGMA_FOR_BILATERAL_FILTER = 70;

		const int INTEGER_KERNEL_SIZE = 7;
		const cv::Size BLUR_KERNEL_SIZE = cv::Size(3, 3);
		
	public:

		Preprocess();

		void ResizeImage(const cv::Mat& inputImage, cv::Mat& outputImage, const int width, const int height);
		void ConvertImageToGray(const cv::Mat& inputImage, cv::Mat& outputImage);
		void NoiseReduction(const cv::Mat& inputImage, cv::Mat& outputImage, const SmoothingAlgorithm smoothingAlgorithm);
		void AdaptiveHistogramEqualization(const cv::Mat& inputImage, cv::Mat& outputImage);

		void SkewCorrection(cv::Mat& inputImage, cv::Mat& outputImage);
		void Undistortion(cv::Mat& inputImage, cv::Mat& outputImage);

		void PreProcessForHaarCascade(const cv::Mat& inputImage, cv::Mat& outputImage);

		void DetectEdges(const cv::Mat& inputImage, cv::Mat& outputImage, const int xDirection, const int yDirection);
		void GetVerticalEdges(cv::Mat& inputOutputImage);

	};

}