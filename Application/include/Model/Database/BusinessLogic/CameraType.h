#pragma once

#include <Model/Database/DataAccess/CameraType.h>

namespace DatabaseBusinessLogic {

	class CameraType {

	private:

		uint8_t MAXIMUM_NUMBER_OF_ENTRIES = 3;

	public:

		CameraType();
		CameraType(const QString& usedDatabase);

		// Table should have 3 entries and each entry should have different Type!
		bool areTableEntriesValid(const std::vector<DatabaseEntity::CameraType>& entries);
		
		std::vector<DatabaseEntity::CameraType> GetAllValidEntries();

		void CheckTableValidation();

		std::vector<DatabaseEntity::CameraType> FindAll();
		DatabaseEntity::CameraType FindByID(const uint32_t id);
		DatabaseEntity::CameraType FindByType(const DatabaseEntity::CameraType::Type type);

	private:

		DatabaseDataAccess::CameraType m_dataAccess;

	};

}