#pragma once

#include <Model/Database/Entities/ParkingSession.h>

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

namespace DatabaseDataAccess {

	class ParkingSession {
	public:

		ParkingSession(const QString& usedDatabase);

		void Add(const DatabaseEntity::ParkingSession& parkingSession);
		void Update(const DatabaseEntity::ParkingSession& parkingSession);

		std::vector<DatabaseEntity::ParkingSession> FindAll();
		DatabaseEntity::ParkingSession FindByID(const uint32_t id);
		std::vector<DatabaseEntity::ParkingSession> FindBySessionID(const uint32_t sessionID);

	private:

		QString m_usedDatabase;
	};

}