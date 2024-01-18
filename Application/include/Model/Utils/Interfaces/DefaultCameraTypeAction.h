#pragma once

#include <Model/Utils/Interfaces/ICameraTypeCustomAction.h>
#include <Model/Database/Database.h>

class DefaultCameraTypeAction : public ICameraTypeCustomAction {
public:

	DefaultCameraTypeAction();

	void DoEntrance(const DatabaseEntity::Camera& camera);
	void DoExit(const DatabaseEntity::Camera& camera, const DatabaseEntity::Session& session);
	void DoParking(const DatabaseEntity::Camera& camera, const DatabaseEntity::Session& session);

private:

	Database m_database;

};