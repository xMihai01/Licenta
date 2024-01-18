#include "Model/Database/BusinessLogic/ParkingSession.h"


void DatabaseBusinessLogic::ParkingSession::Add(DatabaseEntity::ParkingSession& parkingSession)
{
	m_dataAccess.Add(parkingSession);
}

void DatabaseBusinessLogic::ParkingSession::Update(const DatabaseEntity::ParkingSession& parkingSession)
{
	m_dataAccess.Update(parkingSession);
}

std::vector<DatabaseEntity::ParkingSession> DatabaseBusinessLogic::ParkingSession::FindAll()
{
	return m_dataAccess.FindAll();
}

DatabaseEntity::ParkingSession DatabaseBusinessLogic::ParkingSession::FindByID(const uint32_t id)
{
	return m_dataAccess.FindByID(id);
}

std::vector<DatabaseEntity::ParkingSession> DatabaseBusinessLogic::ParkingSession::FindBySessionID(const uint32_t sessionID)
{
	return m_dataAccess.FindBySessionID(sessionID);
}

