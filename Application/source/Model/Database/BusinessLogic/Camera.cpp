#include "Model/Database/BusinessLogic/Camera.h"

bool DatabaseBusinessLogic::Camera::IsLocationAnIndex(const DatabaseEntity::Camera& camera)
{
	return isalnum(camera.GetLocation()[0]);
}
