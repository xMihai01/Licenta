#pragma once

#include <Model/Utils/JsonFile.h>
#include <QString>
#include <vector>

class JsonFileUtils {

public:

	inline const static QString DEFAULT_SLOT_ONE_CELL = "one";
	inline const static QString DEFAULT_SLOT_TWO_CELL = "two";

public:

	static std::vector<QString> ReadDatabaseInfoFromFile(const QString& databaseName);
	static JsonFile GetDefaultCamerasJsonFile();

};