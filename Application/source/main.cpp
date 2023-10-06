#include <Model/VideoCamera.h>

#include <iostream>

int main() {
	
	VideoCamera videoCamera;
	videoCamera.OpenCamera();
	while (videoCamera.IsCameraOpened()) {
		cv::imshow("Frame", videoCamera.GetCurrentFrame());
		if (cv::waitKey(30) == 27)
			break;
	}
	videoCamera.StopCamera();
	
	return 0;
}