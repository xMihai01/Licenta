#include "Model/Database/BusinessLogic/ParkingSession.h"


DatabaseEntity::ParkingSession DatabaseBusinessLogic::ParkingSession::FindOngoingParkingSessionBySessionID(const uint32_t sessionID)
{
	std::vector<DatabaseEntity::ParkingSession> parkingSessions = FindBySessionID(sessionID);
	for (const auto& parkingSession : parkingSessions)
		if (parkingSession.GetEndTime() == NOTIME)
			return parkingSession;
	return DatabaseEntity::ParkingSession();
}

void DatabaseBusinessLogic::ParkingSession::Add(DatabaseEntity::ParkingSession& parkingSession)
{
	parkingSession.SetEndTime(NOTIME);
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

