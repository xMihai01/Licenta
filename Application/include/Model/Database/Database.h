#pragma once

#include <Model/Utils.h>
#include <Model/Database/DataAccess/CameraType.h>

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

private:

    void Validations();

private:

	QSqlDatabase m_database;

    DatabaseDataAccess::CameraType m_cameraType;

	bool isConncted = false;
};