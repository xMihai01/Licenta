#pragma once

#include <Model/Database/Entities/Session.h>

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

namespace DatabaseDataAccess {

	class Session {
	public:

		void Add(const DatabaseEntity::Session& session);
		void Remove(const DatabaseEntity::Session& session);
		void Update(const DatabaseEntity::Session& session);

		std::vector<DatabaseEntity::Session> FindAll();
		DatabaseEntity::Session FindByID(const uint32_t id);
		std::vector<DatabaseEntity::Session> FindByLicensePlate(const std::string& licensePlate);
		DatabaseEntity::Session FindBySecretID(const std::string& secretID);
		std::vector<DatabaseEntity::Session> FindAllOngoingSessions();

	};

}