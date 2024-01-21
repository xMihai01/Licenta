#include "Model/Database/Entities/ParkingSession.h"

DatabaseEntity::ParkingSession::ParkingSession()
{
	m_id = 0;
}

DatabaseEntity::ParkingSession::ParkingSession(const uint32_t id, const uint32_t sessionID, const uint32_t parkingCameraID, const uint32_t parkingSpaceID, const QDateTime& startTime, const QDateTime& endTime)
	:m_id(id)
	, m_sessionID(sessionID)
	, m_parkingCameraID(parkingCameraID)
	, m_parkingSpaceID(parkingSpaceID)
	, m_startTime(startTime)
	, m_endTime(endTime)
{
}

DatabaseEntity::ParkingSession::ParkingSession(const uint32_t sessionID, const uint32_t parkingCameraID, const uint32_t parkingSpaceID, const QDateTime& startTime, const QDateTime& endTime)
	: m_sessionID(sessionID)
	, m_parkingCameraID(parkingCameraID)
	, m_parkingSpaceID(parkingSpaceID)
	, m_startTime(startTime)
	, m_endTime(endTime)
{
	m_id = 0;
}

uint32_t DatabaseEntity::ParkingSession::GetID() const
{
	return m_id;
}

uint32_t DatabaseEntity::ParkingSession::GetSessionID() const
{
	return m_sessionID;
}

uint32_t DatabaseEntity::ParkingSession::GetParkingCameraID() const
{
	return m_parkingCameraID;
}

uint32_t DatabaseEntity::ParkingSession::GetParkingSpaceID() const
{
	return m_parkingSpaceID;
}

QDateTime DatabaseEntity::ParkingSession::GetStartTime() const
{
	return m_startTime;
}

QDateTime DatabaseEntity::ParkingSession::GetEndTime() const
{
	return m_endTime;
}

void DatabaseEntity::ParkingSession::SetSessionID(const uint32_t sessionID)
{
	m_sessionID = sessionID;
}

void DatabaseEntity::ParkingSession::SetParkingCameraID(const uint32_t cameraID)
{
	m_parkingCameraID = cameraID;
}

void DatabaseEntity::ParkingSession::SetParkingSpaceID(const uint32_t spaceID)
{
	m_parkingSpaceID = spaceID;
}

void DatabaseEntity::ParkingSession::SetStartTime(const QDateTime& startTime)
{
	m_startTime = startTime;
}

void DatabaseEntity::ParkingSession::SetEndTime(const QDateTime& endTime)
{
	m_endTime = endTime;
}
