#include "Model/Database/BusinessLogic/CameraKey.h"

void DatabaseBusinessLogic::CameraKey::Add(const DatabaseEntity::CameraKey& cameraKey)
{
    m_dataAccess.Add(cameraKey);
}

void DatabaseBusinessLogic::CameraKey::Remove(const DatabaseEntity::CameraKey& cameraKey)
{
    m_dataAccess.Remove(cameraKey);
}

void DatabaseBusinessLogic::CameraKey::Update(const DatabaseEntity::CameraKey& cameraKey)
{
    m_dataAccess.Update(cameraKey);

}

DatabaseEntity::CameraKey DatabaseBusinessLogic::CameraKey::FindByID(const uint32_t id)
{
    return m_dataAccess.FindByID(id);
}

DatabaseEntity::CameraKey DatabaseBusinessLogic::CameraKey::FindByKey(const QtKeyEnum key)
{
    return m_dataAccess.FindByKey(key);
}

std::vector<DatabaseEntity::CameraKey> DatabaseBusinessLogic::CameraKey::FindAll()
{
    return m_dataAccess.FindAll();
}

bool DatabaseBusinessLogic::CameraKey::IsKeyUsed(const QtKeyEnum key)
{
    return FindByKey(key).GetID() != 0;
}
