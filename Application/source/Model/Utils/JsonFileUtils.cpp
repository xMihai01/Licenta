#include <Model/Utils/JsonFileUtils.h>

std::vector<QString> JsonFileUtils::ReadDatabaseInfoFromFile(const QString& databaseName)
{
	std::vector<QString> cells{ "QtDriver", "ServerName", "DatabaseName", "Username", "Password" };
	JsonFile file(databaseFileName, "database", cells);

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
	return JsonFile(camerasFileName, camerasKey, std::vector<QString>{"camera"});
}

JsonFile JsonFileUtils::GetChosenDetectionTypeFile()
{
	return JsonFile(detectionTypesFileName, detectionTypesKey, std::vector<QString>{"value"});
}

void JsonFileUtils::CheckChosenDetectionTypeFile()
{
	JsonFile file(detectionTypesFileName, detectionTypesKey, std::vector<QString>{"value"});

	auto detectionsInfoEntrance = file.ReadFromFile(detectionTypesKey[0]);
	auto detectionsInfoExit = file.ReadFromFile(detectionTypesKey[1]);
	auto detectionsInfoParking = file.ReadFromFile(detectionTypesKey[2]);
	if (detectionsInfoEntrance.size() + detectionsInfoExit.size() + detectionsInfoParking.size() == 0) {
		file.WriteToFile(std::vector<QString>{QString::number(static_cast<int>(LicensePlateDetection::DetectionType::IMAGE_PROCESSING))}, detectionTypes[0]); // entrance
		file.WriteToFile(std::vector<QString>{QString::number(static_cast<int>(LicensePlateDetection::DetectionType::IMAGE_PROCESSING))}, detectionTypes[1]); // exit
		file.WriteToFile(std::vector<QString>{QString::number(static_cast<int>(LicensePlateDetection::DetectionType::DNN))}, detectionTypes[2]); // parking
		return;
	}
	if (detectionsInfoEntrance.size() + detectionsInfoExit.size() + detectionsInfoParking.size() < detectionTypes.size()*2)
		throw std::runtime_error("Missing cells in detections.json. Delete this file and restart the application!");
	
}

LicensePlateDetection::DetectionType JsonFileUtils::ReadDetectionTypeForCameraType(const DatabaseEntity::CameraType::Type& cameraType)
{
	JsonFile file = GetChosenDetectionTypeFile();
	return ConvertCellFromDetectionTypeFileToDetectionTypeEnum(file.ReadFromFile(detectionTypes[static_cast<int>(cameraType)-1]));
}

void JsonFileUtils::UpdateDetectionTypeInFile(const DatabaseEntity::CameraType::Type& cameraType, const QString& selectedDetectionType)
{
	JsonFile file = GetChosenDetectionTypeFile();
	file.UpdateElement(std::vector<QString>{
		(selectedDetectionType == "DNN" ? "2" : "0")} // 2 is the integer value for detection type DNN while 0 is for image processing
	, detectionTypes[static_cast<int>(cameraType)-1]);
}

LicensePlateDetection::DetectionType JsonFileUtils::ConvertCellFromDetectionTypeFileToDetectionTypeEnum(const std::vector<QString>& cellText)
{
	try {
		int detectionCode = std::stoi(cellText[1].toStdString());
		return static_cast<LicensePlateDetection::DetectionType>(detectionCode);
	}
	catch (...) {
		return LicensePlateDetection::DetectionType::DNN;
	}
}
