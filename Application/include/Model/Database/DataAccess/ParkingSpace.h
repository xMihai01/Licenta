#pragma once

#include <Model/Database/Entities/ParkingSpace.h>

#include <QSqlQuery>
#include <QSqlError>

#include <iostream>
#include <vector>

namespace DatabaseDataAccess {

	class ParkingSpace {

	public:

		ParkingSpace(const QString& usedDatabase);

		void Add(const DatabaseEntity::ParkingSpace& parkingSpace);
		void Remove(const DatabaseEntity::ParkingSpace& parkingSpace);
		void RemoveAllFromCamera(const DatabaseEntity::Camera& camera);
		void Update(const DatabaseEntity::ParkingSpace& parkingSpace);

		std::vector<DatabaseEntity::ParkingSpace> FindAll();
		std::vector<DatabaseEntity::ParkingSpace> FindAllByCamera(const DatabaseEntity::Camera& camera);
		DatabaseEntity::ParkingSpace FindByID(const uint32_t id);

	private:

		QString m_usedDatabase;
	};

}