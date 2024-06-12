#include <Model/Database/BusinessLogic/ParkingSpace.h>

DatabaseBusinessLogic::ParkingSpace::ParkingSpace()
    : m_dataAccess(DatabaseDataAccess::ParkingSpace("main"))
{
}

DatabaseBusinessLogic::ParkingSpace::ParkingSpace(const QString& usedDatabase)
    : m_dataAccess(DatabaseDataAccess::ParkingSpace(usedDatabase))
{
}

void DatabaseBusinessLogic::ParkingSpace::Add(const DatabaseEntity::ParkingSpace& parkingSpace)
{
    m_dataAccess.Add(parkingSpace);
}

void DatabaseBusinessLogic::ParkingSpace::Remove(const DatabaseEntity::ParkingSpace& parkingSpace)
{
    m_dataAccess.Remove(parkingSpace);
}

void DatabaseBusinessLogic::ParkingSpace::RemoveAllFromCamera(const DatabaseEntity::Camera& camera)
{
    m_dataAccess.RemoveAllFromCamera(camera);
}

void DatabaseBusinessLogic::ParkingSpace::Update(const DatabaseEntity::ParkingSpace& parkingSpace)
{
    m_dataAccess.Update(parkingSpace);
}

std::vector<DatabaseEntity::ParkingSpace> DatabaseBusinessLogic::ParkingSpace::FindAll()
{
    return m_dataAccess.FindAll();
}

std::vector<DatabaseEntity::ParkingSpace> DatabaseBusinessLogic::ParkingSpace::FindAllByCamera(const DatabaseEntity::Camera& camera)
{
    return m_dataAccess.FindAllByCamera(camera);
}

DatabaseEntity::ParkingSpace DatabaseBusinessLogic::ParkingSpace::FindByID(const uint32_t id)
{
    return m_dataAccess.FindByID(id);
}