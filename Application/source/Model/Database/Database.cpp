#include <Model/Database/Database.h>

void Database::Connect(const QString& databaseName)
{
	try {
		if (isConncted)
			throw std::runtime_error("Already connnected to database!");

		auto databaseDetails = JsonFileUtils::ReadDatabaseInfoFromFile(databaseName);

		m_database = QSqlDatabase::addDatabase(databaseDetails[1]);
		m_database.setHostName(databaseDetails[2]);
		m_database.setDatabaseName(databaseDetails[3]);
		m_database.setUserName(databaseDetails[4]);
		m_database.setPassword(databaseDetails[5]);

		if (m_database.open(databaseDetails[4], databaseDetails[5])) { // username, password
			std::cout << "Connection to database established.";
			isConncted = true;
			Validations();
		}
		else {
			throw std::runtime_error(m_database.lastError().text().toStdString());
		}
	}
	catch (const std::exception& exception) {
		throw exception;
	}
}

QSqlDatabase Database::GetDatabase() const
{
	return m_database;
}

DatabaseDataAccess::CameraKey Database::ToCameraKey() const
{
	return m_cameraKey;
}

DatabaseDataAccess::CameraType Database::ToCameraType() const
{
	return m_cameraType;
}

DatabaseDataAccess::Camera Database::ToCamera() const
{
	return m_camera;
}

DatabaseBusinessLogic::ParkingSpace Database::ToParkingSpace() const
{
	return m_parkingSpace;
}

DatabaseBusinessLogic::Session Database::ToSession() const
{
	return m_session;
}

DatabaseBusinessLogic::ParkingSession Database::ToParkingSession() const
{
	return m_parkingSession;
}

void Database::Validations()
{
	try {
		m_cameraType.CheckTableValidation();
	}
	catch (const std::exception& exception) {
		throw exception;
	}
}
