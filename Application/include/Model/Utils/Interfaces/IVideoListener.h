#pragma once

//!  IVideoListener interface
/*!
	Interface used to take a frame from the video stream and update it differently.
*/
class IVideoListener {

public:

	/*!
		@brief Updates an object using the captured frame from a video stream

		While the video camera is opened, every time a frame is read, the VideoCamera object will call this method for all listener object inheriting this interface. 

		@param mat Reference to a Mat taken from the video stream
	*/
	virtual void Update(const cv::Mat& mat) = 0;

};