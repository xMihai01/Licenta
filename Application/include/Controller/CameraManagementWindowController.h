#pragma once

#include <Model/VideoCamera.h>

#include <Model/Utils/Interfaces/InterfaceVideoListener.h>
#include <Model/Utils/ImageConversion.h>

#include <Model/Database/Database.h>

#include <QString>
#include <QHash>
#include <QListWidgetItem>

class CameraManagementWindowController {

public:

	enum class CameraManagementMode {
		ADD,
		REMOVE,
		UPDATE,
		LIST
	};

public:

	CameraManagementWindowController();

	void GetAllCamerasFromDatabaseInAMap(QHash<QListWidgetItem*, DatabaseEntity::Camera>& map);

	void DoAction(const DatabaseEntity::Camera& camera);
	void AddOrUpdateKey(const uint32_t id, const QtKeyEnum key);

	void TestCamera(const QString& cameraLocation);

	void UpdateDefaultCamera(const DatabaseEntity::Camera& camera, const bool isSlotOne);

	CameraManagementMode GetCurrentMode() const;
	void SetCurrentMode(const CameraManagementMode mode);

	Database GetDatabase() const;
	
private:

	VideoCamera m_testCamera;

	CameraManagementMode m_currentMode;

	Database m_database;

};