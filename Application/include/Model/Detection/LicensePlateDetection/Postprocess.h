#include <opencv2/opencv.hpp>

namespace LicensePlateDetection {

	class Postprocess {

	public:

		Postprocess(const cv::Mat& image);



	private:

		cv::Mat m_originalImage;

	};

}