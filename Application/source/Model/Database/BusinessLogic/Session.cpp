#include "Model/Database/BusinessLogic/Session.h"

void DatabaseBusinessLogic::Session::ForceExitForSessionID(const uint32_t sessionID)
{
	DatabaseEntity::ParkingSession parkingSession = m_parkingSessionBL.FindOngoingParkingSessionBySessionID(sessionID);
	if (parkingSession.GetID() != 0) {
		parkingSession.SetEndTime(QDateTime::currentDateTime());
		m_parkingSessionBL.Update(parkingSession);
	}
	DatabaseEntity::Session session = m_dataAccess.FindByID(sessionID);
	if (session.GetID() != 0) {
		session.SetExitTime(QDateTime::currentDateTime());
		m_dataAccess.Update(session);
	}
}

std::vector<DatabaseEntity::Session> DatabaseBusinessLogic::Session::FindAllOngoingSessions()
{
	return m_dataAccess.FindAllOngoingSessions();
}

DatabaseEntity::Session DatabaseBusinessLogic::Session::FindValidSessionByLicensePlate(const std::string& licensePlate)
{
	std::vector<DatabaseEntity::Session> sessions = m_dataAccess.FindByLicensePlate(licensePlate);
	for (const auto& session : sessions)
		if (session.GetExitTime() == NOTIME)
			return session;
	return DatabaseEntity::Session();
}

void DatabaseBusinessLogic::Session::Add(DatabaseEntity::Session& session)
{
	session.SetSecretID(GenerateSecretID(session));
	session.SetExitTime(NOTIME);
	m_dataAccess.Add(session);
}

void DatabaseBusinessLogic::Session::Remove(const DatabaseEntity::Session& session)
{
	m_dataAccess.Remove(session);
}

void DatabaseBusinessLogic::Session::Update(const DatabaseEntity::Session& session)
{
	m_dataAccess.Update(session);
}

std::vector<DatabaseEntity::Session> DatabaseBusinessLogic::Session::FindAll()
{
	return m_dataAccess.FindAll();
}

DatabaseEntity::Session DatabaseBusinessLogic::Session::FindByID(const uint32_t id)
{
	return m_dataAccess.FindByID(id);
}

std::vector<DatabaseEntity::Session> DatabaseBusinessLogic::Session::FindByLicensePlate(const std::string& licensePlate)
{
	return m_dataAccess.FindByLicensePlate(licensePlate);
}

DatabaseEntity::Session DatabaseBusinessLogic::Session::FindBySecretID(const std::string& secretID)
{
	return m_dataAccess.FindBySecretID(secretID);
}

std::string DatabaseBusinessLogic::Session::GenerateSecretID(const DatabaseEntity::Session& session)
{
	QString timeInSecs = QString::number(session.GetEntranceTime().toSecsSinceEpoch());
	QString timeInMSecs = QString::number(session.GetEntranceTime().toMSecsSinceEpoch());
	std::string secretID = timeInSecs.toStdString();
	for (size_t index = timeInMSecs.length(); index >= timeInMSecs.length() - 3; index--)
		secretID += std::to_string(timeInMSecs[index].toLatin1());
	return secretID + session.GetLicensePlate();
}
