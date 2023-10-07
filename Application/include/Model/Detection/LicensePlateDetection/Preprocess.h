#include <opencv2/opencv.hpp>

#include "SmoothingAlgorithmEnum.h"

namespace LicensePlateDetection {

	class Preprocess {

	public:

		Preprocess(const cv::Mat& image);

		void ResizeImage(const int width, const int height);
		void ConvertImageToGray();
		void ConvertImageToGray(const SmoothingAlgorithm smoothingAlgorithm);
		void DetectEdges(const double minValue, const double maxValue);

		cv::Mat GetOriginalImage() const;
		cv::Mat GetPreprocessedImage() const;


	private:

		cv::Mat m_preProcessedImage;
		cv::Mat m_originalImage;

	};

}