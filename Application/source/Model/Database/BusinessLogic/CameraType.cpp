#include "Model/Database/BusinessLogic/CameraType.h"

DatabaseBusinessLogic::CameraType::CameraType()
	: m_dataAccess(DatabaseDataAccess::CameraType("main"))
{
}

DatabaseBusinessLogic::CameraType::CameraType(const QString& usedDatabase)
	: m_dataAccess(DatabaseDataAccess::CameraType(usedDatabase))
{
}

bool DatabaseBusinessLogic::CameraType::areTableEntriesValid(const std::vector<DatabaseEntity::CameraType>& entries)
{
	if (entries.size() != MAXIMUM_NUMBER_OF_ENTRIES)
		return false;

	std::unordered_set<DatabaseEntity::CameraType::Type> allTypes;
	for (const auto& entry : entries) {
		if (DatabaseEntity::CameraType::ConvertTypeToQString(entry.GetType()) == DatabaseEntity::CameraType::INVALID_CAMERA_TYPE)
			return false;

		allTypes.insert(entry.GetType());
	}
	if (allTypes.size() != DatabaseEntity::CameraType::qStringToTypesHashmap.size())
		return false;
	return true;
}

std::vector<DatabaseEntity::CameraType> DatabaseBusinessLogic::CameraType::GetAllValidEntries()
{
	std::vector<DatabaseEntity::CameraType> entries;
	entries.push_back(DatabaseEntity::CameraType(1, DatabaseEntity::CameraType::Type::ENTRANCE));
	entries.push_back(DatabaseEntity::CameraType(2, DatabaseEntity::CameraType::Type::EXIT));
	entries.push_back(DatabaseEntity::CameraType(3, DatabaseEntity::CameraType::Type::PARKING));
	
	return entries;
}

void DatabaseBusinessLogic::CameraType::CheckTableValidation()
{
    try {
        if (!areTableEntriesValid(FindAll())) {
            std::cout << "Detected invalid table entries! If this is the first time running the application, ignore this!";
            m_dataAccess.RemoveAll();
            for (const auto& entry : GetAllValidEntries())
                m_dataAccess.Add(entry, false);
        }
        std::cout << "CameraType table was successfully validated!";
    }
    catch (const std::exception& exception) {
        throw exception;
    }
}

std::vector<DatabaseEntity::CameraType> DatabaseBusinessLogic::CameraType::FindAll()
{
	return m_dataAccess.FindAll();
}

DatabaseEntity::CameraType DatabaseBusinessLogic::CameraType::FindByID(const uint32_t id)
{
    return m_dataAccess.FindByID(id);
}

DatabaseEntity::CameraType DatabaseBusinessLogic::CameraType::FindByType(const DatabaseEntity::CameraType::Type type)
{
	return m_dataAccess.FindByID(static_cast<int>(type));
}


