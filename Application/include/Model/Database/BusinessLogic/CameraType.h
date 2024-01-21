#pragma once

#include <Model/Database/Entities/CameraType.h>

#include <stdexcept>
#include <unordered_set>

#include <QHash>
#include <QString>

namespace DatabaseBusinessLogic {

	class CameraType {

	private:

		uint8_t MAXIMUM_NUMBER_OF_ENTRIES = 3;
		QString INVALID_CAMERA_TYPE = "INVALID";

		QHash<QString, DatabaseEntity::CameraType::Type> qStringToTypesHashmap{
		{"ENTRANCE", DatabaseEntity::CameraType::Type::ENTRANCE},
		{"EXIT", DatabaseEntity::CameraType::Type::EXIT},
		{"PARKING", DatabaseEntity::CameraType::Type::PARKING}
		};

	public:

		// Table should have 3 entries and each entry should have different Type!
		bool areTableEntriesValid(const std::vector<DatabaseEntity::CameraType>& entries);
		
		std::vector<DatabaseEntity::CameraType> GetAllValidEntries();

		DatabaseEntity::CameraType::Type ConvertQStringToType(const QString& typeName);
		QString ConvertTypeToQString(const DatabaseEntity::CameraType::Type type);

	};

}