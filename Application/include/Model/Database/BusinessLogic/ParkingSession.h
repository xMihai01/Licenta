#pragma once

#include <Model/Database/DataAccess/ParkingSession.h>

namespace DatabaseBusinessLogic {

	class ParkingSession {
	private:

		inline static const QDateTime NOTIME = QDateTime::fromSecsSinceEpoch(0);

	public:

		void Add(DatabaseEntity::ParkingSession& parkingSession);
		void Update(const DatabaseEntity::ParkingSession& parkingSession);

		std::vector<DatabaseEntity::ParkingSession> FindAll();
		DatabaseEntity::ParkingSession FindByID(const uint32_t id);
		std::vector<DatabaseEntity::ParkingSession> FindBySessionID(const uint32_t sessionID);

	private:

		DatabaseDataAccess::ParkingSession m_dataAccess;

	};

}