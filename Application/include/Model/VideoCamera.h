#pragma once
#include <opencv2/opencv.hpp>

class VideoCamera {

public:

	VideoCamera();

	void OpenCamera();
	void StopCamera();

	cv::Mat GetCurrentFrame();

	bool IsCameraOpened();
	int GetFrameHeight() const;
	int GetFrameWidth() const;

private:

	cv::VideoCapture m_videoCapture;
	cv::Mat m_frame;

	int m_frameHeight;
	int m_frameWidth;
};


