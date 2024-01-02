#pragma once

#include <Model/VideoCamera.h>

#include <Model/Detection/LicensePlateDetection/Workflow.h>

#include <Model/Utils/Interfaces/InterfaceVideoListener.h>
#include <Model/Utils/ImageConversion.h>

#include <Model/Database/Database.h>

#include <QApplication>
#include <QLabel>

class MainWindowController {

public:

	MainWindowController(QLabel* labelForEntranceCameraFrame, QLabel* labelForExitCameraFrame);

	void TakeEntranceFrame();
	void TakeExitFrame();

	void SetupCameras();

	~MainWindowController();

private:

	std::string TakePlateFromFrame(const cv::Mat& frame);

private:

	VideoCamera* m_entranceVideoCameras;
	VideoCamera* m_exitVideoCameras;
	std::vector<VideoCamera> m_parkingVideoCameras;

	std::vector<std::shared_ptr<InterfaceVideoListener>> m_videoListeners;

	LicensePlateDetection::Workflow m_licenseWorkflow;

	QLabel* m_labelForEntranceCameraFrame;
	QLabel* m_labelForExitCameraFrame;

};