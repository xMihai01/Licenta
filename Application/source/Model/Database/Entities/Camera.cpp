#include "Model/Database/Entities/Camera.h"

DatabaseEntity::Camera::Camera()
{
}

DatabaseEntity::Camera::Camera(const DatabaseEntity::CameraType& cameraType, const std::string& location, const std::string& name)
	: m_cameraType(cameraType)
	, m_location(location)
	, m_name(name)
{
	m_id = 0;
}

DatabaseEntity::Camera::Camera(const uint32_t id, const DatabaseEntity::CameraType& cameraType, const std::string& location, const std::string& name)
	: m_id(id)
	, m_cameraType(cameraType)
	, m_location(location)
	, m_name(name)
{
}

uint32_t DatabaseEntity::Camera::GetID() const
{
	return m_id;
}

DatabaseEntity::CameraType DatabaseEntity::Camera::GetCameraType() const
{
	return m_cameraType;
}

std::string DatabaseEntity::Camera::GetLocation() const
{
	return m_location;
}

std::string DatabaseEntity::Camera::GetName() const
{
	return m_name;
}

void DatabaseEntity::Camera::SetCameraType(const DatabaseEntity::CameraType& cameraType)
{
	m_cameraType = cameraType;
}

void DatabaseEntity::Camera::SetLocation(const std::string& location)
{
	m_location = location;
}

void DatabaseEntity::Camera::SetName(const std::string& name)
{
	m_name = name;
}

bool DatabaseEntity::Camera::IsLocationAnIndex() const
{
	return isLocationAnIndex;
}

void DatabaseEntity::Camera::SetIsLocationAnIndex(bool isindex)
{
	isLocationAnIndex = isindex;
}

bool DatabaseEntity::Camera::CheckIsLocationAnIndex(const DatabaseEntity::Camera& camera)
{
	try {
		int test = std::stoi(camera.GetLocation());
		return true;
	}
	catch (...) {
		return false;
	}
}