#include "Model/Database/BusinessLogic/Camera.h"

DatabaseBusinessLogic::Camera::Camera()
    : m_dataAccess(DatabaseDataAccess::Camera("main"))
{
}

DatabaseBusinessLogic::Camera::Camera(const QString& usedDatabase)
    : m_dataAccess(DatabaseDataAccess::Camera(usedDatabase))
{
}

void DatabaseBusinessLogic::Camera::Add(const DatabaseEntity::Camera& camera)
{
    m_dataAccess.Add(camera);
}

void DatabaseBusinessLogic::Camera::Remove(const DatabaseEntity::Camera& camera)
{
    m_dataAccess.Remove(camera);
}

void DatabaseBusinessLogic::Camera::Update(const DatabaseEntity::Camera& camera)
{
    m_dataAccess.Update(camera);
}

std::vector<DatabaseEntity::Camera> DatabaseBusinessLogic::Camera::FindAll()
{
    return m_dataAccess.FindAll();
}

DatabaseEntity::Camera DatabaseBusinessLogic::Camera::FindByID(const uint32_t id)
{
	return m_dataAccess.FindByID(id);
}
