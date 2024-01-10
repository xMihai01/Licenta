#pragma once

#include <Model/Utils/JsonFileUtils.h>
#include <Model/Database/DataAccess/CameraType.h>
#include <Model/Database/DataAccess/Camera.h>

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

    DatabaseDataAccess::CameraType ToCameraType() const;
	DatabaseDataAccess::Camera ToCamera() const;

private:

    void Validations();

private:

	QSqlDatabase m_database;

    DatabaseDataAccess::CameraType m_cameraType;
	DatabaseDataAccess::Camera m_camera;

	bool isConncted = false;
};