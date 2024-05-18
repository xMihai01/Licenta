#pragma once

#include <Model/Utils/Interfaces/DefaultCameraTypeAction.h>
#include <Model/Detection/LicensePlateDetection/Workflow.h>
#include <Model/Database/Database.h>

class ActionManagement {

public:

	static inline LicensePlateDetection::Workflow licensePlateDetectionWorkFlow;

public:

	ActionManagement(const bool requiresDifferentDatabase = false);

	void StartAction(const cv::Mat& frame, const DatabaseEntity::Camera& camera);

	void AddListener(std::shared_ptr<ICameraTypeCustomAction> listener);
	void RemoveListener(std::shared_ptr<ICameraTypeCustomAction> listener);

private:

	void CheckAllParkingSpaces(const DatabaseEntity::Camera& camera, const cv::Mat& inputFrame, const LicensePlateDetection::DetectionType selectedDetectionType);

	void NotifyListeners(const DatabaseEntity::Camera& camera, const std::string& detectedLicensePlate, const DatabaseEntity::ParkingSpace& parkingSpace = DatabaseEntity::ParkingSpace());

private:

	std::vector<std::shared_ptr<ICameraTypeCustomAction>> m_listeners;

	Database m_database;

};