#pragma once

#include <Model/Utils/Interfaces/ICameraTypeCustomAction.h>
#include <Model/Database/Database.h>

class DefaultCameraTypeAction : public ICameraTypeCustomAction {
public:

	DefaultCameraTypeAction();

	void DoEntrance(const DatabaseEntity::Camera& camera, DatabaseEntity::Session& session);
	void DoExit(const DatabaseEntity::Camera& camera, DatabaseEntity::Session& session);
	void DoParking(const DatabaseEntity::Camera& camera, DatabaseEntity::Session& session, const DatabaseEntity::ParkingSpace& parkingSpace);

private:

	Database m_database;

};