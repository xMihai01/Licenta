#pragma once

#include <Model/Utils/JsonFileUtils.h>
#include <Model/Database/BusinessLogic/CameraKey.h>
#include <Model/Database/BusinessLogic/CameraType.h>
#include <Model/Database/BusinessLogic/Camera.h>
#include <Model/Database/BusinessLogic/ParkingSpace.h>
#include <Model/Database/BusinessLogic/ParkingSession.h>
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

	DatabaseBusinessLogic::CameraKey ToCameraKey() const;
    DatabaseBusinessLogic::CameraType ToCameraType() const;
	DatabaseBusinessLogic::Camera ToCamera() const;
	DatabaseBusinessLogic::ParkingSpace ToParkingSpace() const;
	DatabaseBusinessLogic::Session ToSession() const;
	DatabaseBusinessLogic::ParkingSession ToParkingSession() const;

private:

    void Validations();

private:

	QSqlDatabase m_database;

	DatabaseBusinessLogic::CameraKey m_cameraKey;
    DatabaseBusinessLogic::CameraType m_cameraType;
	DatabaseBusinessLogic::Camera m_camera;

	DatabaseBusinessLogic::ParkingSpace m_parkingSpace;
	DatabaseBusinessLogic::ParkingSession m_parkingSession;
	DatabaseBusinessLogic::Session m_session;

	bool isConncted = false;
};