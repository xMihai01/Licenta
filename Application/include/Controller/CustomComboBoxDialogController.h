#pragma once

#include <QHash>

#include <Model/Database/Database.h>

class CustomComboBoxDialogController {
public:

	void GetComboBoxMapForCameras(QHash<QString, DatabaseEntity::Camera>& map);
	void GetComboBoxMapForExitCameras(QHash<QString, DatabaseEntity::Camera>& map);
	void GetComboBoxMapForOngoingSessions(QHash<QString, DatabaseEntity::Session>& map);
	void GetComboBoxMapForCameraType(QHash<QString, DatabaseEntity::CameraType>& map);

private:

	Database m_database;

};