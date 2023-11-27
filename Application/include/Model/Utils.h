#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include <fstream>

class Utils {

private:

	static const int HISTOGRAM_WIDTH = 512;
	static const int HISTOGRAM_HEIGHT = 600;

public:

	static const int INPUT_WIDTH = 640.0;
	static const int INPUT_HEIGHT = 640.0;
	inline static const std::string PLATE_MODEL_NAME = "license_weights_640trained.onnx";
	inline static const std::string PLATE_MODEL_CLASS_LIST = "license_classList.txt";

	static const int PLATE_WIDTH = 400;
	static const int PLATE_HEIGHT = 150;
	static const int MAXIMUM_CONTOUR_AREA = 4000;
	static const int MINIMUM_CONTOUR_AREA = 100;

public:

	static std::pair<cv::Mat, cv::Mat> GetHistograms(const cv::Mat grayImage);
	static std::vector<std::string> GetImageNamesFromFile(const std::string& path);
	static void GetImageByHighestContour(const cv::Mat& inputImage, cv::Mat& outputImage, std::vector<cv::Point>& maxContour
		, const bool crop = false, const std::vector<std::vector<cv::Point>>& customContours = std::vector<std::vector<cv::Point>>{});
	static void BitwiseCharImage(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst);
	static void BitwiseLicensePlateImage(const cv::Mat& blankLicensePlate, const cv::Mat& thresholdedLicensePlate, cv::Mat& outputPlate);

	// Comparators

	static bool letterLocationComparator(std::pair<cv::Mat, cv::Rect>& a, std::pair<cv::Mat, cv::Rect>& b);
	static bool pointComparatorByX(cv::Point& a, cv::Point& b);

};