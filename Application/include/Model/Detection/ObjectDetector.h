#pragma once
#include <opencv2/opencv.hpp>
#include <fstream>
#include <unordered_map>

class ObjectDetector {

	// Detection model types.
	enum class DetectionModelType {
		ONNX, 
		PYTORCH,
		TFLITE
	};

	const std::unordered_map<std::string, DetectionModelType> EXTENSION_TO_MODEL_TYPE_MAP
		= { std::make_pair("onnx", DetectionModelType::ONNX), std::make_pair("pt", DetectionModelType::PYTORCH),
			std::make_pair("tflite", DetectionModelType::TFLITE) };

	// Constants.
	float INPUT_WIDTH = 224.0;
	float INPUT_HEIGHT = 224.0;
	const float SCORE_THRESHOLD = 0.5;
	const float NMS_THRESHOLD = 0.45;
	float CONFIDENCE_THRESHOLD = 0.25;

	const std::string RESOURCES_PATH = "../Resources/";

	// Text parameters.
	const float FONT_SCALE = 0.5;
	const int FONT_FACE = cv::FONT_HERSHEY_SIMPLEX;
	const int THICKNESS = 0.7;

	// Colors.
	cv::Scalar BLACK = cv::Scalar(0, 0, 0);
	cv::Scalar BLUE = cv::Scalar(255, 178, 50);
	cv::Scalar YELLOW = cv::Scalar(0, 255, 255);
	cv::Scalar RED = cv::Scalar(0, 0, 255);

	std::vector<std::string> m_classList;
	std::string m_modelName;
	bool m_isModelAvailable;

	cv::dnn::Net m_net;

	void DrawLabel(cv::Mat& inputImage, std::string& label, int left, int top);
	std::vector<cv::Mat> PreProcess(cv::Mat& inputImage);
	cv::Mat PostProcess(cv::Mat& inputImage, std::vector<cv::Mat>& outputs);

	void ReadClassListFromTxtFile(const std::string& classListFileName);
	DetectionModelType GetDetectionModelTypeFromModelName(const std::string& modelName);

public:

	ObjectDetector();

	bool IsModelReady();

	cv::Mat Detect(const cv::Mat& source);
	void ChangeDetectionModel(const std::string& modelName, const std::string& classListFileName, float inputWidth, float inputHeight, float confidenceThreshold);

}; // https://learnopencv.com/object-detection-using-yolov5-and-opencv-dnn-in-c-and-python/