#pragma once

#include <Model/Database/DataAccess/Camera.h>

namespace DatabaseBusinessLogic {

	class Camera {

	public:

		bool IsLocationAnIndex(const DatabaseEntity::Camera& camera);

		void Add(const DatabaseEntity::Camera& camera);
		void Remove(const DatabaseEntity::Camera& camera);
		void Update(const DatabaseEntity::Camera& camera);

		std::vector<DatabaseEntity::Camera> FindAll();
		DatabaseEntity::Camera FindByID(const uint32_t id);

	private:

		DatabaseDataAccess::Camera m_dataAccess;

	};

}