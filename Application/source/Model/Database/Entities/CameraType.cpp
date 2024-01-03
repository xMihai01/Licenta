#include "Model/Database/Entities/CameraType.h"

DatabaseEntity::CameraType::CameraType(const uint32_t id, const Type type)
	: m_id(id)
	, m_type(type)
{
}

DatabaseEntity::CameraType::CameraType(const Type type)
	: m_type(type)
{
	m_id = 0;
}

DatabaseEntity::CameraType::CameraType()
{
}

void DatabaseEntity::CameraType::SetType(const Type type)
{
	m_type = type;
}

DatabaseEntity::CameraType::Type DatabaseEntity::CameraType::GetType() const
{
	return m_type;
}

uint32_t DatabaseEntity::CameraType::GetID() const
{
	return m_id;
}
