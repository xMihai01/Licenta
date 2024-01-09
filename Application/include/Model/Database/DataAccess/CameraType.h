#pragma once

#include <Model/Database/BusinessLogic/CameraType.h>

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include <iostream>

namespace DatabaseDataAccess {

	class CameraType {

	public:

		CameraType();

		void CheckTableValidation();

		std::vector<DatabaseEntity::CameraType> FindAll();
		DatabaseEntity::CameraType FindByID(const uint32_t id);
		DatabaseEntity::CameraType FindByType(const DatabaseEntity::CameraType::Type type);

		DatabaseBusinessLogic::CameraType ToBusinessLogic() const;

	private:

		void Remove(const DatabaseEntity::CameraType& cameraType);
		void RemoveAll();

		void Add(const DatabaseEntity::CameraType& cameraType, const bool ignoreId = true);
		
	private:

		DatabaseBusinessLogic::CameraType m_businessLogic;

	};

}