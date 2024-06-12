#pragma once

#include <stdint.h>
#include <QDebug>
#include <QHash>
#include <QString>

namespace DatabaseEntity {

	class CameraType {
		
	public:

		enum class Type {
			ENTRANCE = 1,
			EXIT = 2,
			PARKING = 3,
			INVALID = 0
		};

		static inline QString INVALID_CAMERA_TYPE = "INVALID";

		static inline QHash<QString, DatabaseEntity::CameraType::Type> qStringToTypesHashmap{
		{"ENTRANCE", DatabaseEntity::CameraType::Type::ENTRANCE},
		{"EXIT", DatabaseEntity::CameraType::Type::EXIT},
		{"PARKING", DatabaseEntity::CameraType::Type::PARKING}
		};

	public:

		CameraType(const uint32_t id, const Type type);
		CameraType(const Type type);
		CameraType();

		void SetType(const Type type);
		
		Type GetType() const;
		uint32_t GetID() const;

	private:

		uint32_t m_id;
		Type m_type;

	public:

		static DatabaseEntity::CameraType::Type ConvertQStringToType(const QString& typeName);
		static QString ConvertTypeToQString(const DatabaseEntity::CameraType::Type type);

	};

}