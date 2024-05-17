#pragma once

#include <stdint.h>

#include <Model/Database/DataAccess/CameraKey.h>

namespace DatabaseBusinessLogic {

	class CameraKey {
		
	public:

		DatabaseEntity::CameraKey FindByID(const uint32_t id);
		DatabaseEntity::CameraKey FindByKey(const QtKeyEnum key);
		std::vector<DatabaseEntity::CameraKey> FindAll();

		void Add(const DatabaseEntity::CameraKey& cameraKey);
		void Remove(const DatabaseEntity::CameraKey& cameraKey);
		void Update(const DatabaseEntity::CameraKey& cameraKey);

		bool IsKeyUsed(const QtKeyEnum key);


	private:

		DatabaseDataAccess::CameraKey m_dataAccess;
	};

}