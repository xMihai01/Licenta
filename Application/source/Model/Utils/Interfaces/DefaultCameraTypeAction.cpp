#include "Model/Utils/Interfaces/DefaultCameraTypeAction.h"

DefaultCameraTypeAction::DefaultCameraTypeAction(const Database& database)
	: m_database(database)
{
}

void DefaultCameraTypeAction::DoEntrance(const DatabaseEntity::Camera& camera, DatabaseEntity::Session& session)
{
	// if license plate's length is smaller than 5 chars, make it empty as it is not valid.
	if (session.GetLicensePlate().size() < 5)
		session.SetLicensePlate("");

	if (m_database.ToSession().FindValidSessionByLicensePlate(session.GetLicensePlate()).GetID() != 0 && session.GetLicensePlate().size() >= 5) {
		qDebug() << "\nCouldn't create new session as there is already an ongoing session with this license plate. If this is a mistake, force exit session with ID: " 
			<< m_database.ToSession().FindValidSessionByLicensePlate(session.GetLicensePlate()).GetID();
		return;
	}

	session.SetEntranceTime(QDateTime::currentDateTime());
	m_database.ToSession().Add(session);
	qDebug() << "\nCreated new session" << "\nLicense plate: " << QString::fromStdString(session.GetLicensePlate())
		<< "\nSecret ID: " << QString::fromStdString(session.GetSecretID());
}

void DefaultCameraTypeAction::DoExit(const DatabaseEntity::Camera& camera, DatabaseEntity::Session& session)
{
	if (session.GetID() == 0 || session.GetLicensePlate() == "") {
		qDebug() << "\nCouldn't detect current session beucase the license plate was not detected. Please use SecretID to exit.";
		return;
	}

	DatabaseEntity::ParkingSession parkingSession = m_database.ToParkingSession().FindOngoingParkingSessionBySessionID(session.GetID());
	if (parkingSession.GetID() != 0) {
		parkingSession.SetEndTime(QDateTime::currentDateTime());
		m_database.ToParkingSession().Update(parkingSession);
	}

	session.SetExitTime(QDateTime::currentDateTime());
	m_database.ToSession().Update(session);
	qDebug() << "\nEnded session with ID: " << session.GetID() << "\nLicense plate: " << QString::fromStdString(session.GetLicensePlate());
}

void DefaultCameraTypeAction::DoParking(const DatabaseEntity::Camera& camera, DatabaseEntity::Session& session, const DatabaseEntity::ParkingSpace& parkingSpace)
{
	DatabaseEntity::ParkingSession parkingSession = m_database.ToParkingSession().FindOngoingParkingSessionBySessionID(session.GetID());

	// Driver is already parked here/license plate was not found in any ongoing sessions.
	if (session.GetID() == 0 || session.GetLicensePlate() == "") {
		return;
	}
	if (parkingSession.GetParkingSpaceID() == parkingSpace.GetID()) {
		return;
	}

	// If parking session is different than 0, the driver is already parked on a certain space.
	// Mark the previous parking session as ended, and create a new one.
	if (!parkingSession.GetID() == 0) {
		parkingSession.SetEndTime(QDateTime::currentDateTime());
		m_database.ToParkingSession().Update(parkingSession);
	}
	parkingSession = DatabaseEntity::ParkingSession(session.GetID(), parkingSpace.GetCamera().GetID(), parkingSpace.GetID(), QDateTime::currentDateTime());
	m_database.ToParkingSession().Add(parkingSession);
}
