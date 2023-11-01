#include <Model/Detection/LicensePlateDetection/Postprocess.h>
#include <Model/Detection/LicensePlateDetection/Preprocess.h>
#include <Model/Detection/ObjectDetector.h>
#include <Model/Utils.h>

namespace LicensePlateDetection {

	enum class DetectionType {
		IMAGE_PROCESSING = 0,
		HAAR_CASCADE = 1,
		DNN = 2
	};

	class Workflow {

	public:

		Workflow();

		void Detect(cv::Mat& inputImage, cv::Mat& outputImage, std::string& outputText, const LicensePlateDetection::DetectionType detectionType);
		// for testing
		void DetectMultiple(const LicensePlateDetection::DetectionType detectionType, const std::string& fileName);

	private:

		LicensePlateDetection::Postprocess m_postprocessing;
		LicensePlateDetection::Preprocess m_preprocessing;

		ObjectDetector m_detector;
	};

}