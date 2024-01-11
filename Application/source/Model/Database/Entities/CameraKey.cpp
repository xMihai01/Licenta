#include "Model/Database/Entities/CameraKey.h"

DatabaseEntity::CameraKey::CameraKey(const uint32_t id, const QtKeyEnum key)
	: m_id(id)
	, m_key(key)
{
}

DatabaseEntity::CameraKey::CameraKey(const QtKeyEnum key)
	: m_key(key)
{
	m_id = 0;
}

DatabaseEntity::CameraKey::CameraKey()
{
	m_id = 0;
}

void DatabaseEntity::CameraKey::SetKey(const QtKeyEnum Key)
{
	m_key = Key;
}

QtKeyEnum DatabaseEntity::CameraKey::GetKey() const
{
	return m_key;
}

uint32_t DatabaseEntity::CameraKey::GetID() const
{
	return m_id;
}
