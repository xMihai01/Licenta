#pragma once

#include <Model/Database/DataAccess/ParkingSpace.h>

namespace DatabaseBusinessLogic {

	class ParkingSpace {
	public:

		ParkingSpace();
		ParkingSpace(const QString& usedDatabase);
		
		void Add(const DatabaseEntity::ParkingSpace& parkingSpace);
		void Remove(const DatabaseEntity::ParkingSpace& parkingSpace);
		void RemoveAllFromCamera(const DatabaseEntity::Camera& camera);
		void Update(const DatabaseEntity::ParkingSpace& parkingSpace);

		std::vector<DatabaseEntity::ParkingSpace> FindAll();
		std::vector<DatabaseEntity::ParkingSpace> FindAllByCamera(const DatabaseEntity::Camera& camera);
		DatabaseEntity::ParkingSpace FindByID(const uint32_t id);

	private:

		DatabaseDataAccess::ParkingSpace m_dataAccess;
	};

}