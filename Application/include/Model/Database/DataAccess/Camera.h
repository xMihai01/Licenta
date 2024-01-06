#pragma once

#include <Model/Database/BusinessLogic/Camera.h>
#include <Model/Database/DataAccess/CameraType.h>

#include <QSqlQuery>
#include <QSqlError>

#include <iostream>
#include <vector>

namespace DatabaseDataAccess {

	class Camera {

	public:

		Camera();

		void Add(const DatabaseEntity::Camera& camera);
		void Remove(const DatabaseEntity::Camera& camera);
		void Update(const DatabaseEntity::Camera& camera);

		std::vector<DatabaseEntity::Camera> FindAll();
		DatabaseEntity::Camera FindByID(const uint32_t id);

	private:

		DatabaseBusinessLogic::Camera m_businessLogic;

	};

}