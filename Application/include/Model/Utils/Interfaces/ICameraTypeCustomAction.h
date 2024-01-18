#pragma once

#include <Model/Database/Entities/Camera.h>
#include <Model/Database/Entities/Session.h>

class ICameraTypeCustomAction {

public:

	virtual void DoEntrance(const DatabaseEntity::Camera& camera) = 0;
	virtual void DoExit(const DatabaseEntity::Camera& camera, const DatabaseEntity::Session& session) = 0;
	virtual void DoParking(const DatabaseEntity::Camera& camera, const DatabaseEntity::Session& session) = 0;

};