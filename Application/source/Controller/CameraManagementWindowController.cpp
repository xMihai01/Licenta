#include <Controller/CameraManagementWindowController.h>

CameraManagementWindowController::CameraManagementWindowController()
{
	m_currentMode = CameraManagementMode::ADD;
}

void CameraManagementWindowController::GetAllCamerasFromDatabaseInAMap(QHash<QListWidgetItem*, DatabaseEntity::Camera>& map)
{
	if (map.size() > 0) {
		for (const auto& item : map.asKeyValueRange()) {
			delete item.first;
		}
	} // if it crashes here, remove everything above
	std::vector<DatabaseEntity::Camera> cameras = m_database.ToCamera().FindAll();

	for (const auto& camera : cameras) {
		map[new QListWidgetItem()] = camera;
	}
}

void CameraManagementWindowController::DoAction(const DatabaseEntity::Camera& camera)
{
	try {
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
	catch (const std::exception& exception) {
		throw exception;
	}
}

void CameraManagementWindowController::AddOrUpdateKey(const uint32_t id, const QtKeyEnum key)
{
	DatabaseEntity::CameraKey cameraKey = m_database.ToCameraKey().FindByID(id);
	if (cameraKey.GetID() == 0)
		m_database.ToCameraKey().Add(DatabaseEntity::CameraKey(id, key));
	else if (cameraKey.GetID() != 0)
		m_database.ToCameraKey().Update(DatabaseEntity::CameraKey(id, key));
}

void CameraManagementWindowController::TestCamera(const QString& cameraLocation)
{
	try {
		VideoCamera testVideoCamera;
		DatabaseEntity::Camera testCamera(-1, DatabaseEntity::CameraType::Type::INVALID, cameraLocation.toStdString(), "");
		testCamera.SetIsLocationAnIndex(DatabaseEntity::Camera::CheckIsLocationAnIndex(testCamera));
		testCamera.IsLocationAnIndex() ? testVideoCamera.OpenCamera(cameraLocation.toInt()) : testVideoCamera.OpenCamera(cameraLocation.toStdString());
		testVideoCamera.StopCamera();
	}
	catch (const std::exception& exception) {
		throw exception;
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
