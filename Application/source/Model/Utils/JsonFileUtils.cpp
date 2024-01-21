#include <Model/Utils/JsonFileUtils.h>

std::vector<QString> JsonFileUtils::ReadDatabaseInfoFromFile(const QString& databaseName)
{
	std::vector<QString> cells{ "QtDriver", "ServerName", "DatabaseName", "Username", "Password" };
	JsonFile file("database.json", "database", cells);

	auto databaseInfo = file.ReadFromFile(databaseName);
	if (databaseInfo.size() == 0) {
		file.WriteToFile(std::vector<QString>{"QPSQL", "localhost", "your_db_name", "your_username", "your_password"}, "main");
		databaseInfo = file.ReadFromFile(databaseName);
	}
	if (databaseInfo.size() < cells.size() + 1)
		throw std::runtime_error("Missing cells in database.json. Delete this file and restart the application!");
	return databaseInfo;
}

JsonFile JsonFileUtils::GetDefaultCamerasJsonFile()
{
	return JsonFile("cameras.json", "slot", std::vector<QString>{"camera"});
}
