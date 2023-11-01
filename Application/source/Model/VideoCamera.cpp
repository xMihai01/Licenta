#include "Model/VideoCamera.h"

VideoCamera::VideoCamera()
{

}

void VideoCamera::OpenCamera()
{
	m_videoCapture.open(0);

	if (!m_videoCapture.isOpened()) {
		throw std::runtime_error("Couldn't open camera");
	}

	m_frameWidth = m_videoCapture.get(cv::CAP_PROP_FRAME_WIDTH);
	m_frameHeight = m_videoCapture.get(cv::CAP_PROP_FRAME_HEIGHT);
}

void VideoCamera::StopCamera()
{
	m_frame = NULL;
	m_videoCapture.release();
	cv::destroyAllWindows();
}

cv::Mat VideoCamera::GetCurrentFrame()
{
	if (m_videoCapture.isOpened()) {
		m_videoCapture.read(m_frame);
		return m_frame;
	}
	return cv::Mat();
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

