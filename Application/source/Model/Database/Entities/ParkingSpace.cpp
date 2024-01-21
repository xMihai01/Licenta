#include <Model/Database/Entities/ParkingSpace.h>

DatabaseEntity::ParkingSpace::ParkingSpace()
{
	m_id = 0;
	m_camera = DatabaseEntity::Camera();
	m_x1 = -1;
	m_x2 = -1;
	m_y1 = -1;
	m_y2 = -1;
	m_name = "";
}

DatabaseEntity::ParkingSpace::ParkingSpace(const uint32_t id, const DatabaseEntity::Camera& camera, const uint32_t x1, const uint32_t x2, const uint32_t y1, const uint32_t y2, const std::string& name)
	: m_id(id)
	, m_camera(camera)
	, m_x1(x1)
	, m_x2(x2)
	, m_y1(y1)
	, m_y2(y2)
	, m_name(name) {}

DatabaseEntity::ParkingSpace::ParkingSpace(const DatabaseEntity::Camera& camera, const uint32_t x1, const uint32_t x2, const uint32_t y1, const uint32_t y2, const std::string& name)
	: m_camera(camera)
	, m_x1(x1)
	, m_x2(x2)
	, m_y1(y1)
	, m_y2(y2)
	, m_name(name)
{
	m_id = 0;
}

void DatabaseEntity::ParkingSpace::SetX1(const uint32_t x1)
{
	m_x1 = x1;
}

void DatabaseEntity::ParkingSpace::SetX2(const uint32_t x2)
{
	m_x2 = x2;
}

void DatabaseEntity::ParkingSpace::SetY1(const uint32_t y1)
{
	m_y1 = y1;
}

void DatabaseEntity::ParkingSpace::SetY2(const uint32_t y2)
{
	m_y2 = y2;
}

void DatabaseEntity::ParkingSpace::SetName(const std::string& name)
{
	m_name = name;
}

uint32_t DatabaseEntity::ParkingSpace::GetID() const
{
	return m_id;
}

DatabaseEntity::Camera DatabaseEntity::ParkingSpace::GetCamera() const
{
	return m_camera;
}

uint32_t DatabaseEntity::ParkingSpace::GetX1() const
{
	return m_x1;
}

uint32_t DatabaseEntity::ParkingSpace::GetX2() const
{
	return m_x2;
}

uint32_t DatabaseEntity::ParkingSpace::GetY1() const
{
	return m_y1;
}

uint32_t DatabaseEntity::ParkingSpace::GetY2() const
{
	return m_y2;
}

std::string DatabaseEntity::ParkingSpace::GetName() const
{
	return m_name;
}
