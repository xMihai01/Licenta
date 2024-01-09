#pragma once

#include <View/cameramanagementwindow.h>

#include <Model/Detection/LicensePlateDetection/Workflow.h>

#include <QApplication>
#include <QLabel>

class MainWindowController {

public:

	MainWindowController(QLabel* labelForEntranceCameraFrame, QLabel* labelForExitCameraFrame);


	void OpenCameraManagementWindow(const CameraManagementWindowController::CameraManagementMode mode);


	void TakeEntranceFrame();
	void TakeExitFrame();

	void SetupCameras();

	void Close();

	~MainWindowController();

private:

	std::string TakePlateFromFrame(const cv::Mat& frame);

private:

	VideoCamera* m_entranceVideoCameras;
	VideoCamera* m_exitVideoCameras;
	std::vector<VideoCamera> m_parkingVideoCameras;

	std::vector<std::shared_ptr<InterfaceVideoListener>> m_videoListeners;

	CameraManagementWindow* m_cameraManagementWindow;

	LicensePlateDetection::Workflow m_licenseWorkflow;

	QLabel* m_labelForEntranceCameraFrame;
	QLabel* m_labelForExitCameraFrame;

	Database m_database;

};