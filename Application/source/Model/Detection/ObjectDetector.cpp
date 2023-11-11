#include "Model/Detection/ObjectDetector.h"


void ObjectDetector::DrawLabel(cv::Mat& inputImage, std::string& label, int left, int top)
{
    int baseLine;
    cv::Size label_size = cv::getTextSize(label, FONT_FACE, FONT_SCALE, THICKNESS, &baseLine);
    top = cv::max(top, label_size.height);

    cv::Point tlc = cv::Point(left, top);
    cv::Point brc = cv::Point(left + label_size.width, top + label_size.height + baseLine);

    cv::rectangle(inputImage, tlc, brc, BLACK, cv::FILLED);
    cv::putText(inputImage, label, cv::Point(left, top + label_size.height), FONT_FACE, FONT_SCALE, YELLOW, THICKNESS);
}

std::vector<cv::Mat> ObjectDetector::PreProcess(cv::Mat& inputImage)
{

    // put the image in a square big enough
    int col = inputImage.cols;
    int row = inputImage.rows;
    int _max = MAX(col, row);
    cv::Mat resized = cv::Mat::zeros(_max, _max, CV_8UC3);
    inputImage.copyTo(resized(cv::Rect(0, 0, col, row)));

    // resize to 640x640, normalize to [0,1[ and swap Red and Blue channels
    cv::Mat blob;
    cv::dnn::blobFromImage(inputImage, blob, 1. / 255., cv::Size(INPUT_WIDTH, INPUT_HEIGHT), cv::Scalar(), true, false);

    m_net.setInput(blob);
    //std::vector<cv::String> outNames = m_net.getUnconnectedOutLayersNames();

    //std::vector<cv::Mat> outputs;
    std::vector<cv::Mat> predictions;
    m_net.forward(predictions, m_net.getUnconnectedOutLayersNames());
    const cv::Mat& output = predictions[0];
//    m_net.forward(outputs);
    return predictions;
}

cv::Mat ObjectDetector::PostProcess(cv::Mat& inputImage, std::vector<cv::Mat>& outputs)
{
    // Initialize vectors to hold respective outputs while unwrapping detections.
    std::vector<int> class_ids;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;
    // Resizing factor.
    float x_factor = inputImage.cols / INPUT_WIDTH;
    float y_factor = inputImage.rows / INPUT_HEIGHT;
    float* data = (float*)outputs[0].data;
    int dimensions = m_classList.size();
    int rows = outputs[0].size[1];

    for (int i = 0; i < rows; ++i)
    {
        float confidence = data[4];
        if (confidence >= CONFIDENCE_THRESHOLD && confidence <= 1.00)
        {
            float* classes_scores = data + 5;
            cv::Mat scores(1, m_classList.size(), CV_32FC1, classes_scores);

            cv::Point class_id;
            double max_class_score;
            cv::minMaxLoc(scores, 0, &max_class_score, 0, &class_id);

            confidences.push_back(confidence);
            class_ids.push_back(class_id.x);
            // Center.
            float cx = data[0];
            float cy = data[1];
            // Box dimension.
            float w = data[2];
            float h = data[3];
            // Bounding box coordinates.
            int left = int((cx - 0.5 * w) * x_factor);
            int top = int((cy - 0.5 * h) * y_factor);
            int width = int(w * x_factor);
            int height = int(h * y_factor);

            boxes.push_back(cv::Rect(left, top, width, height));
        }
        // Jump to the next row.
        data += (5 + dimensions);
    }
    // Perform Non-Maximum Suppression and draw predictions.
    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confidences, SCORE_THRESHOLD, NMS_THRESHOLD, indices);
    cv::Rect maxRect = cv::Rect(0,0,0,0);
    for (int i = 0; i < indices.size(); i++)
    {
        int idx = indices[i];
        cv::Rect box = boxes[idx];
        int left = box.x;
        int top = box.y;
        int width = box.width;
        int height = box.height;
        // Draw bounding box.
        // TODO: Refactor this.
        //return inputImage(cv::Rect(left, top, width, height));
        if (width * height > maxRect.width * maxRect.height)
            maxRect = cv::Rect(left, top, width, height);
        //rectangle(inputImage, cv::Point(left, top), cv::Point(left + width, top + height), BLUE, 3 * THICKNESS);
        //DrawLabel(inputImage, m_classList[class_ids[idx]] + ":" + cv::format("%.2f", confidences[idx]), left, top);
    }

    return (maxRect.height != 0 && maxRect.width != 0 ) ? inputImage(maxRect) : inputImage;
}

void ObjectDetector::ReadClassListFromTxtFile(const std::string& classListFileName)
{
    std::ifstream classListFileInput(RESOURCES_PATH + classListFileName);
    if (!classListFileInput.good())
        throw std::runtime_error("Class list file '" + classListFileName + "' couldn't be opened. Make sure it exists and it is in the Resources folder!");

    std::string className;
    while (classListFileInput >> className)
        m_classList.push_back(className);

    if (m_classList.size() == 0)
        throw std::runtime_error("The class list must not be empty! Try reviewing " + classListFileName + " and make sure there is a just one class on each row.");
}

ObjectDetector::DetectionModelType ObjectDetector::GetDetectionModelTypeFromModelName(const std::string& modelName)
{
    std::string extension = modelName.substr(modelName.find('.') + 1);
    std::ifstream modelFile(RESOURCES_PATH + modelName);
    if (!modelFile.good())
        throw std::runtime_error("Model '" + modelName + "' couldn't be opened. Make sure it exists and it is in the Resources folder!");

    if (EXTENSION_TO_MODEL_TYPE_MAP.find(extension) != EXTENSION_TO_MODEL_TYPE_MAP.end())
        return EXTENSION_TO_MODEL_TYPE_MAP.at(extension);
    else
        throw std::runtime_error("The '" + extension + "' extension is not supported!");
}

ObjectDetector::ObjectDetector()
{
    m_isModelAvailable = false;
}

cv::Mat ObjectDetector::Detect(const cv::Mat& source)
{
    try {
        cv::Mat frame = source.clone();
        cv::resize(source, frame, cv::Size(INPUT_WIDTH, INPUT_HEIGHT), cv::INTER_LINEAR);

        std::vector<cv::Mat> detections = PreProcess(frame);
        cv::Mat img = PostProcess(frame, detections);

        // Put efficiency information.
        // The function getPerfProfile returns the overall time for     inference(t) and the timings for each of the layers(in layersTimes).
        std::vector<double> layersTimes;
        double freq = cv::getTickFrequency() / 1000;
        double t = m_net.getPerfProfile(layersTimes) / freq;
        std::string label = cv::format("Inference time : %.2f ms", t);
        //cv::putText(img, label, cv::Point(20, 40), FONT_FACE, FONT_SCALE, RED);

        return img;
    }
    catch (const std::exception& exception) {
        throw std::runtime_error("Model was loaded but can't be used for detection. Check input width/height, class list or export the model using ONNX.");
    }
}


void ObjectDetector::ChangeDetectionModel(const std::string& modelName, const std::string& classListFileName, float inputWidth, float inputHeight, float confidenceThreshold)
{
    m_classList.clear();
    INPUT_HEIGHT = inputHeight;
    INPUT_WIDTH = inputWidth;
    CONFIDENCE_THRESHOLD = confidenceThreshold;
    m_isModelAvailable = false;
    try {
        switch (GetDetectionModelTypeFromModelName(modelName))
        {
        case DetectionModelType::ONNX:
            m_net = cv::dnn::readNetFromONNX(RESOURCES_PATH + modelName);
            break;
        case DetectionModelType::PYTORCH:
            m_net = cv::dnn::readNetFromTorch(RESOURCES_PATH + modelName);
            break;
        case DetectionModelType::TFLITE:
            m_net = cv::dnn::readNetFromTFLite(RESOURCES_PATH + modelName);
            break;
        default:
            m_net = cv::dnn::readNet(RESOURCES_PATH + modelName);
            break;
        }
        ReadClassListFromTxtFile(classListFileName);
        Detect(cv::Mat::zeros(inputWidth, inputHeight, CV_8UC3));
        m_isModelAvailable = true;
    }
    catch (const std::exception& exception) {
        throw exception;
    }
}

bool ObjectDetector::IsModelReady()
{
    return m_isModelAvailable;
}
