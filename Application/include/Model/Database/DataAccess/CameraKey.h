#pragma once

#include <stdint.h>

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include <iostream>

#include <Model/Database/BusinessLogic/CameraKey.h>

namespace DatabaseDataAccess {

	class CameraKey {

	public:

		CameraKey();

		//std::vector<DatabaseEntity::CameraType> FindAll();
		DatabaseEntity::CameraKey FindByID(const uint32_t id);
		std::vector<DatabaseEntity::CameraKey> FindAll();

		//DatabaseBusinessLogic::CameraType ToBusinessLogic() const;

		void Add(const DatabaseEntity::CameraKey& cameraKey);
		void Remove(const DatabaseEntity::CameraKey& cameraKey);
		void Update(const DatabaseEntity::CameraKey& cameraKey);

	private:

		DatabaseBusinessLogic::CameraKey m_businessLogic;

	};

}