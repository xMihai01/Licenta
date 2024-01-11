#pragma once

#include <View/cameramanagementwindow.h>

#include <Model/Utils/QtKeysEnum.h>
#include <Model/Detection/LicensePlateDetection/Workflow.h>

#include <QApplication>
#include <QLabel>

class MainWindowController {

public:

	MainWindowController(QLabel* labelForEntranceCameraFrame, QLabel* labelForExitCameraFrame);

	void GetFrameAndStartAction(const uint32_t cameraID);

	void OpenCameraManagementWindow(const CameraManagementWindowController::CameraManagementMode mode);

	void ChangeCameraOnSlot(const DatabaseEntity::Camera& camera, bool isSlotOne);
	
	void SetupCameras();

	void Refresh();

	void Close();

	~MainWindowController();

private:

	std::string TakePlateFromFrame(const cv::Mat& frame);
	void GetDefaultCameras();


private:

	std::vector<VideoCamera*> m_entranceVideoCameras;
	std::vector<VideoCamera*> m_exitVideoCameras;
	std::vector<VideoCamera*> m_parkingVideoCameras;

	std::pair<std::pair<DatabaseEntity::Camera, VideoCamera*>, std::pair<DatabaseEntity::Camera, VideoCamera*>> m_cameraSlot;

	std::unordered_map<uint32_t, VideoCamera*> m_cameraIDToVideoCameraMap;

	std::vector<std::shared_ptr<InterfaceVideoListener>> m_videoListeners;

	CameraManagementWindow* m_cameraManagementWindow;

	LicensePlateDetection::Workflow m_licenseWorkflow;

	QLabel* m_labelForEntranceCameraFrame;
	QLabel* m_labelForExitCameraFrame;

	Database m_database;

};