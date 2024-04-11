#include "Model/ActionManagement.h"

ActionManagement::ActionManagement()
{
	AddListener(std::make_shared<DefaultCameraTypeAction>());
	// Add other custom listeners here 
}

void ActionManagement::StartAction(const cv::Mat& frame, const DatabaseEntity::Camera& camera)
{
    //static int index = 0;
    //cv::imwrite("C:/Users/mihai/Desktop/Products/img/test" + std::to_string(index) + ".jpg", frame);
    //index++;
    cv::Mat inputFrame = frame.clone();
    std::string plateText;
    std::string plateTextIP;
    cv::Mat outputPlateImage;
    switch (camera.GetCameraType().GetType())
    {
    case DatabaseEntity::CameraType::Type::ENTRANCE:
        //m_licenseWorkflow.Detect(inputFrame, outputPlateImage, plateText, LicensePlateDetection::DetectionType::DNN);
        m_licenseWorkflow.Detect(inputFrame, outputPlateImage, plateText, LicensePlateDetection::DetectionType::IMAGE_PROCESSING);
        std::cout << "\nEntered: " << plateText;
        break;
    case DatabaseEntity::CameraType::Type::EXIT:
        //m_licenseWorkflow.Detect(inputFrame, outputPlateImage, plateText, LicensePlateDetection::DetectionType::DNN);
        m_licenseWorkflow.Detect(inputFrame, outputPlateImage, plateText, LicensePlateDetection::DetectionType::IMAGE_PROCESSING);
        std::cout << "\nExited: " << plateText;
        break;
    case DatabaseEntity::CameraType::Type::PARKING:
        std::cout << "\nParked: " << plateText;
        CheckAllParkingSpaces(camera, inputFrame);
        break;
    default:
        break;
    }
    NotifyListeners(camera, plateText);
    //cv::imwrite("C:/Users/mihai/Desktop/Products/img/input_" + std::to_string(index) + ".jpg", inputFrame);
    //cv::imwrite("C:/Users/mihai/Desktop/Products/img/" + std::to_string(index) + ".jpg", outputPlateImage);
    //index++;
}

void ActionManagement::CheckAllParkingSpaces(const DatabaseEntity::Camera& camera, const cv::Mat& inputFrame)
{
    //static int index = 0;
    std::string text = "";
    std::string ipText = "";
    cv::Mat inputFrameCropped;
    cv::Mat outputImage;
    for (const auto& space : m_database.ToParkingSpace().FindAllByCamera(camera)) {
        Utils::CropImageFromRectangle(inputFrame, inputFrameCropped, cv::Point2d(space.GetX1(), space.GetY1()), cv::Point2d(space.GetX2(), space.GetY2()));
        //cv::imwrite("C:/Users/mihai/Desktop/Products/parking_spaces/" + std::to_string(index) + ".jpg", inputFrameCropped);
        //index++;
        m_licenseWorkflow.Detect(inputFrameCropped, outputImage, text, LicensePlateDetection::DetectionType::DNN);
        m_licenseWorkflow.Detect(inputFrameCropped, outputImage, ipText, LicensePlateDetection::DetectionType::IMAGE_PROCESSING);
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
