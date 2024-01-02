#include <Model/Database/Database.h>

void Database::Connect(const std::string& serverName, const std::string& username, const std::string& password)
{
	m_database = QSqlDatabase::addDatabase("QPSQL");
	m_database.setHostName("localhost");
	m_database.setDatabaseName("Licenta");
	m_database.setUserName("postgres");
	m_database.setPassword("admin");
	bool connectioncheck = m_database.open("postgres", "admin");

	if (connectioncheck == true) {
		std::cout << "Connection to database established.";
	}
	else {
		std::cout << "Error :" << m_database.lastError().text().toStdString();
	}
}

QSqlDatabase Database::GetDatabase() const
{
	return m_database;
}
