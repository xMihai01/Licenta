#pragma once

#include <Model/Database/Entities/CameraType.h>

#include <QSqlQuery>
#include <QSqlError>

#include <stdexcept>
#include <unordered_set>
#include <iostream>

namespace DatabaseDataAccess {

	class CameraType {

	public:

		CameraType();

		std::vector<DatabaseEntity::CameraType> FindAll();
		DatabaseEntity::CameraType FindByID(const uint32_t id);
		DatabaseEntity::CameraType FindByType(const DatabaseEntity::CameraType::Type type);

		void Remove(const DatabaseEntity::CameraType& cameraType);
		void RemoveAll();

		void Add(const DatabaseEntity::CameraType& cameraType, const bool ignoreId = true);

	};

}