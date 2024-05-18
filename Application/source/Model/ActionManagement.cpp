#include "Model/ActionManagement.h"

ActionManagement::ActionManagement(const bool requiresDifferentDatabase)
{
    if (requiresDifferentDatabase) {
        std::stringstream ss;
        ss << std::this_thread::get_id();
        std::string threadID = ss.str();
        m_database.ConnectDuplicate("main", QString::fromStdString(threadID));
    }

	AddListener(std::make_shared<DefaultCameraTypeAction>(m_database));
	// Add other custom listeners here 
}

void ActionManagement::StartAction(const cv::Mat& frame, const DatabaseEntity::Camera& camera)
{
    cv::Mat inputFrame = frame.clone();
    std::string plateText;
    std::string plateTextIP;
    cv::Mat outputPlateImage;

    DatabaseEntity::CameraType::Type cameraType = camera.GetCameraType().GetType();
    LicensePlateDetection::DetectionType selectedDetectionType = JsonFileUtils::ReadDetectionTypeForCameraType(cameraType);

    switch (cameraType)
    {
    case DatabaseEntity::CameraType::Type::ENTRANCE:
        ActionManagement::licensePlateDetectionWorkFlow.Detect(inputFrame, outputPlateImage, plateText, selectedDetectionType);
        std::cout << "\nEntered: " << plateText;
        break;
    case DatabaseEntity::CameraType::Type::EXIT:
        ActionManagement::licensePlateDetectionWorkFlow.Detect(inputFrame, outputPlateImage, plateText, selectedDetectionType);
        std::cout << "\nExited: " << plateText;
        break;
    case DatabaseEntity::CameraType::Type::PARKING:
        std::cout << "\nParked: " << plateText;
        CheckAllParkingSpaces(camera, inputFrame, selectedDetectionType);
        break;
    default:
        break;
    }
    NotifyListeners(camera, plateText);
}

void ActionManagement::CheckAllParkingSpaces(const DatabaseEntity::Camera& camera, const cv::Mat& inputFrame, const LicensePlateDetection::DetectionType selectedDetectionType)
{
    std::string text = "";
    std::string ipText = "";
    cv::Mat inputFrameCropped;
    cv::Mat outputImage;
    for (const auto& space : m_database.ToParkingSpace().FindAllByCamera(camera)) {

        Utils::CropImageFromRectangle(inputFrame, inputFrameCropped, cv::Point2d(space.GetX1(), space.GetY1()), cv::Point2d(space.GetX2(), space.GetY2()));
        ActionManagement::licensePlateDetectionWorkFlow.Detect(inputFrameCropped, outputImage, text, selectedDetectionType);
        std::cout << space.GetName() << ": IP: " << ipText << " | DNN: " << text << "\n";

        NotifyListeners(camera, text, space);
    }
}

void ActionManagement::AddListener(std::shared_ptr<ICameraTypeCustomAction> listener)
{
	m_listeners.push_back(listener);
}

void ActionManagement::RemoveListener(std::shared_ptr<ICameraTypeCustomAction> listener)
{
	for (auto iterator = m_listeners.begin(); iterator != m_listeners.end(); )
	{
		if (*iterator == listener)
			iterator = m_listeners.erase(iterator);
		else
			++iterator;
	}
}

void ActionManagement::NotifyListeners(const DatabaseEntity::Camera& camera, const std::string& detectedLicensePlate, const DatabaseEntity::ParkingSpace& parkingSpace)
{
	for (auto& listener : m_listeners) {
        DatabaseEntity::Session session;
       
		switch (camera.GetCameraType().GetType()) {
		case DatabaseEntity::CameraType::Type::ENTRANCE:
            session = DatabaseEntity::Session(detectedLicensePlate);
			listener->DoEntrance(DatabaseEntity::Camera(), session);
			break;
		case DatabaseEntity::CameraType::Type::EXIT:
            session = m_database.ToSession().FindValidSessionByLicensePlate(detectedLicensePlate);
            listener->DoExit(DatabaseEntity::Camera(), session);
			break;
		case DatabaseEntity::CameraType::Type::PARKING:
            session = m_database.ToSession().FindValidSessionByLicensePlate(detectedLicensePlate);
            listener->DoParking(camera, session, parkingSpace);
			break;
		}
	}
}
