#include <Model/Database/Database.h>

void Database::Connect(const QString& databaseName)
{
	try {
		if (isConncted)
			throw std::runtime_error("Already connnected to database!");

		auto databaseDetails = JsonFileUtils::ReadDatabaseInfoFromFile(databaseName);

		m_database = QSqlDatabase::addDatabase(databaseDetails[1], "main");
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

void Database::ConnectDuplicate(const QString& databaseName, const QString& newDatabaseName)
{
	try {
		auto databaseDetails = JsonFileUtils::ReadDatabaseInfoFromFile(databaseName);

		m_database = QSqlDatabase::addDatabase(databaseDetails[1], newDatabaseName);
		m_database.setHostName(databaseDetails[2]);
		m_database.setDatabaseName(databaseDetails[3]);
		m_database.setUserName(databaseDetails[4]);
		m_database.setPassword(databaseDetails[5]);

		if (m_database.open(databaseDetails[4], databaseDetails[5])) { // username, password
			std::cout << "Duplicate connection to database established: " << newDatabaseName.toStdString();
			m_parkingSpace = DatabaseBusinessLogic::ParkingSpace(newDatabaseName);
			m_parkingSession = DatabaseBusinessLogic::ParkingSession(newDatabaseName);
			m_session = DatabaseBusinessLogic::Session(newDatabaseName);
			m_camera = DatabaseBusinessLogic::Camera(newDatabaseName);
			m_cameraKey = DatabaseBusinessLogic::CameraKey(newDatabaseName);
			m_cameraType = DatabaseBusinessLogic::CameraType(newDatabaseName);
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

DatabaseBusinessLogic::CameraKey Database::ToCameraKey() const
{
	return m_cameraKey;
}

DatabaseBusinessLogic::CameraType Database::ToCameraType() const
{
	return m_cameraType;
}

DatabaseBusinessLogic::Camera Database::ToCamera() const
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
