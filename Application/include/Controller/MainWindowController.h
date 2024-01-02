#pragma once

#include <Model/VideoCamera.h>
#include <Model/Detection/LicensePlateDetection/Workflow.h>

#include <QApplication>
#include <QLabel>

class MainWindowController {

public:

	MainWindowController(QLabel* labelForEntranceCameraFrame, QLabel* labelForExitCameraFrame);

	void TakeEntranceFrame();
	void TakeExitFrame();

	void SetupCameras();

private:

	std::string TakePlateFromFrame(const cv::Mat& frame);

private:

	VideoCamera* m_entranceVideoCameras;
	VideoCamera* m_exitVideoCameras;
	std::vector<VideoCamera*> m_parkingVideoCameras;

	LicensePlateDetection::Workflow m_licenseWorkflow;

};