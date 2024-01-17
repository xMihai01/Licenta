#pragma once

#include <Model/Utils/JsonFileUtils.h>
#include <Model/Database/DataAccess/CameraKey.h>
#include <Model/Database/DataAccess/CameraType.h>
#include <Model/Database/DataAccess/Camera.h>
#include <Model/Database/BusinessLogic/ParkingSpace.h>
#include <Model/Database/BusinessLogic/Session.h>

#include <QSql>
#include <QSqlDriver>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include <iostream>

class Database {

public:

	void Connect(const QString& databaseName);

	QSqlDatabase GetDatabase() const;

	DatabaseDataAccess::CameraKey ToCameraKey() const;
    DatabaseDataAccess::CameraType ToCameraType() const;
	DatabaseDataAccess::Camera ToCamera() const;
	DatabaseBusinessLogic::ParkingSpace ToParkingSpace() const;
	DatabaseBusinessLogic::Session ToSession() const;

private:

    void Validations();

private:

	QSqlDatabase m_database;

	DatabaseDataAccess::CameraKey m_cameraKey;
    DatabaseDataAccess::CameraType m_cameraType;
	DatabaseDataAccess::Camera m_camera;

	DatabaseBusinessLogic::ParkingSpace m_parkingSpace;
	DatabaseBusinessLogic::Session m_session;

	bool isConncted = false;
};