#pragma once

#include <Model/Database/Entities/Camera.h>

namespace DatabaseBusinessLogic {

	class Camera {

	public:

		bool IsLocationAnIndex(const DatabaseEntity::Camera& camera);

	};

}