#include "Model/VideoCamera.h"

VideoCamera::VideoCamera()
{}

void VideoCamera::ReadFrames()
{
	while (m_videoCapture.isOpened()) {

		{
			std::lock_guard<std::mutex> lock(frameMutex);
			if (!m_videoCapture.read(m_frame)) {
				break;
			}
			m_useableFrame = m_frame.clone();
			NotifyListeners();
		}
	
		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / FRAME_RATE));
	}
}

void VideoCamera::OpenCamera(const int index)
{
	m_videoCapture.open(index);

	if (!m_videoCapture.isOpened()) {
		throw std::runtime_error("Camera index " + std::to_string(index) + " could not open!");
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

	m_frameWidth = m_videoCapture.get(cv::CAP_PROP_FRAME_WIDTH);
	m_frameHeight = m_videoCapture.get(cv::CAP_PROP_FRAME_HEIGHT);
}

void VideoCamera::StopCamera()
{
	m_frame = cv::Mat();
	m_videoCapture.release();
	cv::destroyAllWindows();
}

cv::Mat VideoCamera::GetCurrentFrame()
{
	if (!m_videoCapture.isOpened())
		throw std::runtime_error("Tried taking the current frame on video camera but it is not opened!");
	cv::Mat frame;
	{
		std::lock_guard<std::mutex> lock(frameMutex);
		frame = m_useableFrame.clone();
	}

	return frame;
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

void VideoCamera::AddListener(std::shared_ptr<IVideoListener> listener)
{
	m_listeners.push_back(listener);
}

void VideoCamera::RemoveListener(std::shared_ptr<IVideoListener> listener)
{
	for (auto iterator = m_listeners.begin(); iterator != m_listeners.end(); )
	{
		if (*iterator == listener)
			iterator = m_listeners.erase(iterator);
		else
			++iterator;
	}
}

void VideoCamera::NotifyListeners()
{
	for (auto& listener : m_listeners)
		listener->Update(m_useableFrame);
}

