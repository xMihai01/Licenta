#pragma once

#include <Model/Database/DataAccess/Session.h>

namespace DatabaseBusinessLogic {

	class Session {
	public:

		// TODO: Add other functions for calculating number of hours stayed in the parking lot, valid parking in valid spaces, etc

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

	};

}