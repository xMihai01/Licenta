#pragma once
#include <opencv2/opencv.hpp>

#include <Model/Utils/Interfaces/IVideoListener.h>

class VideoCamera {

public:

	VideoCamera();
	VideoCamera(const VideoCamera& camera);

	void ReadFrames();

	void OpenCamera(const int index);
	void OpenCamera(const std::string& filename);
	void StopCamera();

	cv::Mat GetCurrentFrame();
	cv::Mat ReadNextFrame();

	bool IsCameraOpened();
	int GetFrameHeight() const;
	int GetFrameWidth() const;

	void AddListener(std::shared_ptr<IVideoListener> listener);
	void RemoveListener(std::shared_ptr<IVideoListener> listener);

private:

	void NotifyListeners();

private:

	cv::VideoCapture* m_videoCapture;
	cv::Mat m_frame;
	cv::Mat m_useableFrame;

	int m_frameHeight;
	int m_frameWidth;

	int FRAME_RATE = 60;

	std::vector<std::shared_ptr<IVideoListener>> m_listeners;
	
	std::mutex frameMutex;

};


