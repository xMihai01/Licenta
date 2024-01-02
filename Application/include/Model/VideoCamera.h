#pragma once
#include <opencv2/opencv.hpp>

class VideoCamera {

public:

	VideoCamera(cv::Mat* frame);

	void OpenCamera(const int index);
	void OpenCamera(const std::string& filename);
	void StopCamera();

	cv::Mat GetCurrentFrame();

	bool IsCameraOpened();
	int GetFrameHeight() const;
	int GetFrameWidth() const;

private:

	cv::VideoCapture m_videoCapture;
	cv::Mat* m_frame;

	int m_frameHeight;
	int m_frameWidth;

	int FRAME_RATE = 30;

	std::mutex m_frameMutex;
};


