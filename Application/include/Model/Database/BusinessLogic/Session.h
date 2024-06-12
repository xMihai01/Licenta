#pragma once

#include <Model/Database/DataAccess/Session.h>
#include <Model/Database/BusinessLogic/ParkingSession.h>

namespace DatabaseBusinessLogic {

	class Session {
	private:

		inline static const QDateTime NOTIME = QDateTime::fromSecsSinceEpoch(0);

	public:

		Session();
		Session(const QString& usedDatabase);
		
		void ForceExitForSessionID(const uint32_t sessionID);
		std::vector<DatabaseEntity::Session> FindAllOngoingSessions();
		DatabaseEntity::Session FindValidSessionByLicensePlate(const std::string& licensePlate);

		void Add(DatabaseEntity::Session& session);
		void Remove(const DatabaseEntity::Session& session);
		void Update(const DatabaseEntity::Session& session);

		std::vector<DatabaseEntity::Session> FindAll();
		DatabaseEntity::Session FindByID(const uint32_t id);
		std::vector<DatabaseEntity::Session> FindByLicensePlate(const std::string& licensePlate);
		DatabaseEntity::Session FindBySecretID(const std::string& secretID);

	private:

		std::string GenerateSecretID(const DatabaseEntity::Session& session);

	private:

		DatabaseDataAccess::Session m_dataAccess;
		DatabaseBusinessLogic::ParkingSession m_parkingSessionBL;

	};

}