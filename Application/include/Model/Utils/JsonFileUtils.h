#pragma once

#include <Model/Utils/JsonFile.h>
#include <Model/Database/BusinessLogic/CameraType.h>
#include <Model/Detection/LicensePlateDetection/DetectionType.h>

#include <QString>
#include <vector>

class JsonFileUtils {

public:

	inline const static QString DEFAULT_SLOT_ONE_CELL = "one";
	inline const static QString DEFAULT_SLOT_TWO_CELL = "two";

	inline const static QString databaseFileName = "database.json";
	inline const static QString camerasFileName = "cameras.json";
	inline const static QString detectionTypesFileName = "detections.json";

	inline const static QString camerasKey = "slot";

	inline const static QString detectionTypesKey = "type";
	inline const static std::vector<QString> detectionTypes{ DatabaseEntity::CameraType::ConvertTypeToQString(DatabaseEntity::CameraType::Type::ENTRANCE)
		, DatabaseEntity::CameraType::ConvertTypeToQString(DatabaseEntity::CameraType::Type::EXIT)
		, DatabaseEntity::CameraType::ConvertTypeToQString(DatabaseEntity::CameraType::Type::PARKING) };

public:

	// database.json
	static std::vector<QString> ReadDatabaseInfoFromFile(const QString& databaseName);

	// cameras.json
	static JsonFile GetDefaultCamerasJsonFile();
	
	// detections.json
	static JsonFile GetChosenDetectionTypeFile();
	static void CheckChosenDetectionTypeFile();
	static LicensePlateDetection::DetectionType ReadDetectionTypeForCameraType(const DatabaseEntity::CameraType::Type& cameraType);
	static void UpdateDetectionTypeInFile(const DatabaseEntity::CameraType::Type& cameraType, const QString& selectedDetectionType);

private:

	static LicensePlateDetection::DetectionType ConvertCellFromDetectionTypeFileToDetectionTypeEnum(const std::vector<QString>& cellText);
};