#include "Model/Detection/LicensePlateDetection/Preprocess.h"

LicensePlateDetection::Preprocess::Preprocess(const cv::Mat& image)
	: m_originalImage(image)
{
	m_preProcessedImage = image.clone();
}

void LicensePlateDetection::Preprocess::ResizeImage(const int width, const int height)
{
	cv::resize(m_preProcessedImage, m_preProcessedImage, cv::Size(width, height));
}

void LicensePlateDetection::Preprocess::ConvertImageToGray()
{
	cv::cvtColor(m_preProcessedImage, m_preProcessedImage, cv::COLOR_BGR2GRAY);
}

void LicensePlateDetection::Preprocess::ConvertImageToGray(const SmoothingAlgorithm smoothingAlgorithm)
{
	ConvertImageToGray();
	cv::Mat tempMat;
	switch (smoothingAlgorithm)
	{
	case SmoothingAlgorithm::BilateralFilter:
		cv::bilateralFilter(m_preProcessedImage, tempMat, 5, 17, 17);
		m_preProcessedImage = tempMat;
		break;
	case SmoothingAlgorithm::Gaussian:
		cv::GaussianBlur(m_preProcessedImage, m_preProcessedImage, cv::Size(5, 5), 0);
		break;
	default:
		break;
	}
} // https://docs.opencv.org/4.x/d4/d13/tutorial_py_filtering.html

void LicensePlateDetection::Preprocess::DetectEdges(const double minValue, const double maxValue)
{
	cv::Canny(m_preProcessedImage, m_preProcessedImage, minValue, maxValue);
}

cv::Mat LicensePlateDetection::Preprocess::GetOriginalImage() const
{
	return m_originalImage;
}

cv::Mat LicensePlateDetection::Preprocess::GetPreprocessedImage() const
{
	return m_preProcessedImage;
}
