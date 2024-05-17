#pragma once

#include <stdint.h>

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include <iostream>

#include <Model/Database/Entities/CameraKey.h>

namespace DatabaseDataAccess {

	class CameraKey {

	public:

		CameraKey();

		DatabaseEntity::CameraKey FindByID(const uint32_t id);
		DatabaseEntity::CameraKey FindByKey(const QtKeyEnum key);
		std::vector<DatabaseEntity::CameraKey> FindAll();

		void Add(const DatabaseEntity::CameraKey& cameraKey);
		void Remove(const DatabaseEntity::CameraKey& cameraKey);
		void Update(const DatabaseEntity::CameraKey& cameraKey);

	};

}