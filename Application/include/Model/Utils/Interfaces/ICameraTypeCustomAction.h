#pragma once

#include <Model/Database/Entities/Camera.h>
#include <Model/Database/Entities/Session.h>
#include <Model/Database/Entities/ParkingSpace.h>

class ICameraTypeCustomAction {

public:

	virtual void DoEntrance(const DatabaseEntity::Camera& camera, DatabaseEntity::Session& session) = 0;
	virtual void DoExit(const DatabaseEntity::Camera& camera, DatabaseEntity::Session& session) = 0;
	virtual void DoParking(const DatabaseEntity::Camera& camera, DatabaseEntity::Session& session, const DatabaseEntity::ParkingSpace& parkingSpace) = 0;

};