#include <Controller/CameraManagementWindowController.h>

CameraManagementWindowController::CameraManagementWindowController()
{
	m_currentMode = CameraManagementMode::ADD;
}

void CameraManagementWindowController::GetAllCamerasFromDatabaseInAMap(QHash<QListWidgetItem*, DatabaseEntity::Camera>& map)
{
	std::vector<DatabaseEntity::Camera> cameras = m_database.ToCamera().FindAll();

	for (const auto& camera : cameras) {
		map[new QListWidgetItem()] = camera;
	}
}

void CameraManagementWindowController::DoAction(const DatabaseEntity::Camera& camera)
{
	switch (m_currentMode)
	{
	case CameraManagementWindowController::CameraManagementMode::ADD:
		m_database.ToCamera().Add(camera);
		break;
	case CameraManagementWindowController::CameraManagementMode::REMOVE:
		m_database.ToCamera().Remove(camera);
		break;
	case CameraManagementWindowController::CameraManagementMode::UPDATE:
		m_database.ToCamera().Update(camera);
		break;
	default:
		break;
	}
}

void CameraManagementWindowController::UpdateDefaultCamera(const DatabaseEntity::Camera& camera, const bool isSlotOne)
{
	JsonFile defaultCameras = JsonFileUtils::GetDefaultCamerasJsonFile();
	isSlotOne ? defaultCameras.UpdateElement(std::vector<QString>{QString::number(camera.GetID())}, JsonFileUtils::DEFAULT_SLOT_ONE_CELL) : defaultCameras.UpdateElement(std::vector<QString>{QString::number(camera.GetID())}, JsonFileUtils::DEFAULT_SLOT_TWO_CELL);
}

CameraManagementWindowController::CameraManagementMode CameraManagementWindowController::GetCurrentMode() const
{
	return m_currentMode;
}

void CameraManagementWindowController::SetCurrentMode(const CameraManagementWindowController::CameraManagementMode mode)
{
	m_currentMode = mode;
}

Database CameraManagementWindowController::GetDatabase() const
{
	return m_database;
}
