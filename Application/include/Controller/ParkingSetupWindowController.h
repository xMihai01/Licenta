#pragma once

#include <Model/Database/Database.h>
#include <Model/VideoCamera.h>
#include <Model/Utils/ImageConversion.h>
#include <Model/Utils.h>
#include <View/CustomComponents/ClickableLabel.h>

#include <QHash>
#include <QListWidget>

class ParkingSetupWindowController {
private:

	const int WIDTH = 640;
	const int HEIGHT = 480;
	double scaleFactorX;
	double scaleFactorY;

public:

	ParkingSetupWindowController(ClickableLabel* label);

	void GetAllParkingCamerasInMap(QHash<QListWidgetItem*, DatabaseEntity::Camera>& map);
	void GetAllSpacesForParkingCamera(QHash<QListWidgetItem*, DatabaseEntity::ParkingSpace>& map, const DatabaseEntity::Camera& camera);

	void ShowParkingSpace(const QPoint& first, const QPoint& second);
	void ShowParkingSpace(const DatabaseEntity::ParkingSpace& parkingSpace);

	void TakeSingleFrameFromCamera(const DatabaseEntity::Camera& camera);

	void AddParkingSpace(const DatabaseEntity::Camera& camera, const QPoint& first, const QPoint& second, const QString& name);
	void UpdateParkingSpace(const DatabaseEntity::ParkingSpace& parkingSpace, const QPoint& first, const QPoint& second, const QString& name);
	void RemoveParkingSpace(const DatabaseEntity::ParkingSpace& parkingSpace);

	void Reset();

private:

	Database m_database;

	ClickableLabel* m_label;
};