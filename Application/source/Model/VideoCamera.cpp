#include "Model/VideoCamera.h"

VideoCamera::VideoCamera(cv::Mat* frame)
{
	m_frame = new cv::Mat();
}

void VideoCamera::OpenCamera(const int index)
{
	m_videoCapture.open(index);

	if (!m_videoCapture.isOpened()) {
		throw std::runtime_error("Camera index " + std::to_string(index) + " could not open!");
	}

	while (m_videoCapture.isOpened()) {
		m_videoCapture.read(*m_frame);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / FRAME_RATE));
	}

	m_frameWidth = m_videoCapture.get(cv::CAP_PROP_FRAME_WIDTH);
	m_frameHeight = m_videoCapture.get(cv::CAP_PROP_FRAME_HEIGHT);
}

void VideoCamera::OpenCamera(const std::string& filename)
{
	m_videoCapture.open(filename);

	if (!m_videoCapture.isOpened()) {
		throw std::runtime_error("Camera " + filename + " could not open!");
	}

	while (m_videoCapture.isOpened()) {
		m_videoCapture.read(*m_frame);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / FRAME_RATE));
	}

	m_frameWidth = m_videoCapture.get(cv::CAP_PROP_FRAME_WIDTH);
	m_frameHeight = m_videoCapture.get(cv::CAP_PROP_FRAME_HEIGHT);
}

void VideoCamera::StopCamera()
{
	//m_frame = NULL;
	delete m_frame;
	m_videoCapture.release();
	cv::destroyAllWindows();
}

cv::Mat VideoCamera::GetCurrentFrame()
{
	if (!m_videoCapture.isOpened())
		throw std::runtime_error("Tried taking the current frame on video camera but it is not opened!");

	return m_frame->clone();
}

bool VideoCamera::IsCameraOpened()
{
	return m_videoCapture.isOpened();
}

int VideoCamera::GetFrameHeight() const
{
	return m_frameHeight;
}

int VideoCamera::GetFrameWidth() const
{
	return m_frameWidth;
}

