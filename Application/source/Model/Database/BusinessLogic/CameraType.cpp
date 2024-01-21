#include "Model/Database/BusinessLogic/CameraType.h"

bool DatabaseBusinessLogic::CameraType::areTableEntriesValid(const std::vector<DatabaseEntity::CameraType>& entries)
{
	if (entries.size() != MAXIMUM_NUMBER_OF_ENTRIES)
		return false;

	std::unordered_set<DatabaseEntity::CameraType::Type> allTypes;
	for (const auto& entry : entries) {
		if (ConvertTypeToQString(entry.GetType()) == INVALID_CAMERA_TYPE)
			return false;

		allTypes.insert(entry.GetType());
	}
	if (allTypes.size() != this->qStringToTypesHashmap.size())
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

DatabaseEntity::CameraType::Type DatabaseBusinessLogic::CameraType::ConvertQStringToType(const QString& typeName)
{
	if (this->qStringToTypesHashmap.find(typeName) == this->qStringToTypesHashmap.end())
		return DatabaseEntity::CameraType::Type::INVALID;
    return this->qStringToTypesHashmap.value(typeName);
}

QString DatabaseBusinessLogic::CameraType::ConvertTypeToQString(const DatabaseEntity::CameraType::Type type)
{
	switch (type)
	{
	case DatabaseEntity::CameraType::Type::ENTRANCE:
		return "ENTRANCE";
	case DatabaseEntity::CameraType::Type::EXIT:
		return "EXIT";
	case DatabaseEntity::CameraType::Type::PARKING:
		return "PARKING";
	default:
		return INVALID_CAMERA_TYPE;
	}
}
