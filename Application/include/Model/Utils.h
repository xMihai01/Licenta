#include <opencv2/opencv.hpp>
#include <vector>
#include <fstream>

class Utils {

private:

	static const int HISTOGRAM_WIDTH = 512;
	static const int HISTOGRAM_HEIGHT = 600;

public:

	static std::pair<cv::Mat, cv::Mat> GetHistograms(const cv::Mat grayImage);
	static std::vector<std::string> GetImageNamesFromFile(const std::string& path);
};