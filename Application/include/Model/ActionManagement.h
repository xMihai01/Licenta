#pragma once

#include <Model/Utils/Interfaces/DefaultCameraTypeAction.h>
#include <Model/Detection/LicensePlateDetection/Workflow.h>
#include <Model/Database/Database.h>

class ActionManagement {
public:

	ActionManagement();

	void StartAction(const cv::Mat& frame, const DatabaseEntity::Camera& camera);

	void AddListener(std::shared_ptr<ICameraTypeCustomAction> listener);
	void RemoveListener(std::shared_ptr<ICameraTypeCustomAction> listener);

private:

	void CheckAllParkingSpaces(const DatabaseEntity::Camera& camera, const cv::Mat& inputFrame);

	void NotifyListeners(const DatabaseEntity::Camera& camera, const std::string& detectedLicensePlate, const DatabaseEntity::ParkingSpace& parkingSpace = DatabaseEntity::ParkingSpace());

private:

	LicensePlateDetection::Workflow m_licenseWorkflow;

	std::vector<std::shared_ptr<ICameraTypeCustomAction>> m_listeners;

	Database m_database;

};