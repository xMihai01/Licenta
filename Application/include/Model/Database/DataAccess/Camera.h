#pragma once

#include <Model/Database/Entities/Camera.h>
#include <Model/Database/DataAccess/CameraType.h>
#include <Model/Database/DataAccess/CameraKey.h>
#include <Model/Database/DataAccess/ParkingSpace.h>

#include <QSqlQuery>
#include <QSqlError>

#include <iostream>
#include <vector>

class DatabaseDataAccess::ParkingSpace;

namespace DatabaseDataAccess {

	class Camera {

	public:

		Camera(const QString& usedDatabase);

		void Add(const DatabaseEntity::Camera& camera);
		void Remove(const DatabaseEntity::Camera& camera);
		void Update(const DatabaseEntity::Camera& camera);

		std::vector<DatabaseEntity::Camera> FindAll();
		DatabaseEntity::Camera FindByID(const uint32_t id);

	private:

		DatabaseDataAccess::CameraKey m_cameraKeyDataAccess = DatabaseDataAccess::CameraKey("main");
		DatabaseDataAccess::ParkingSpace m_parkingSpaceDataAccess = DatabaseDataAccess::ParkingSpace("main");
		DatabaseDataAccess::CameraType m_cameraTypeDataAccess = DatabaseDataAccess::CameraType("main");

		QString m_usedDatabase;

	};

}