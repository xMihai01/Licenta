#include "Controller/MainWindowController.h"

MainWindowController::MainWindowController(QLabel* labelForEntranceCameraFrame, QLabel* labelForExitCameraFrame)
    : m_labelForEntranceCameraFrame(labelForEntranceCameraFrame)
    , m_labelForExitCameraFrame(labelForExitCameraFrame)
{
    try {
        m_database.Connect("main");

        m_cameraManagementWindow = new CameraManagementWindow();
        m_parkingSetupWindow = new ParkingSetupWindow();

        // Since there are only 2 slots, there will be 2 listeners.
        m_videoListeners.push_back(std::make_shared<InterfaceVideoListener>(labelForEntranceCameraFrame));
        m_videoListeners.push_back(std::make_shared<InterfaceVideoListener>(labelForExitCameraFrame));
        
        // loading detection types for each camera type
        JsonFileUtils::CheckChosenDetectionTypeFile();

        SetupCameras();
       
    }
    catch (const std::exception& exception) {
        throw exception;
    }
    
}

void MainWindowController::GetFrameAndStartAction(const uint32_t cameraID)
{
    const DatabaseEntity::Camera camera = m_database.ToCamera().FindByID(cameraID);
    cv::Mat inputFrame = m_cameraIDToVideoCameraMap[cameraID]->GetCurrentFrame();

    m_actionManagement.StartAction(inputFrame, camera);
}

void MainWindowController::OpenCameraManagementWindow(const CameraManagementWindowController::CameraManagementMode mode)
{
    if (m_cameraManagementWindow->isVisible())
        m_cameraManagementWindow->hide();
    m_cameraManagementWindow->ChangeMode(mode);
    m_cameraManagementWindow->show();
}

void MainWindowController::OpenParkingManagementWindow()
{
    if (m_parkingSetupWindow->isVisible())
        m_parkingSetupWindow->hide();
    m_parkingSetupWindow->show();
    m_parkingSetupWindow->ReloadCameras();
}

void MainWindowController::SetupCameras()
{
    VideoCamera* currentCamera;
    try {
        GetDefaultCameras();
        std::vector<DatabaseEntity::Camera> cameras = m_database.ToCamera().FindAll();

        for (const auto& camera : cameras) {
            VideoCamera* videoCamera = new VideoCamera();
            currentCamera = videoCamera;

            camera.IsLocationAnIndex() ? videoCamera->OpenCamera(std::stoi(camera.GetLocation())) : videoCamera->OpenCamera(camera.GetLocation());
            switch (camera.GetCameraType().GetType())
            {
            case DatabaseEntity::CameraType::Type::ENTRANCE:
                m_entranceVideoCameras.push_back(videoCamera);
                break;
            case DatabaseEntity::CameraType::Type::EXIT:
                m_exitVideoCameras.push_back(videoCamera);
                break;
            case DatabaseEntity::CameraType::Type::PARKING:
                m_parkingVideoCameras.push_back(videoCamera);
                break;
            default:
                break;
            }
            if (camera.GetID() == m_cameraSlot.first.first.GetID()) {
                videoCamera->AddListener(m_videoListeners[0]);
                m_cameraSlot.first.second = videoCamera;
            }
            if (camera.GetID() == m_cameraSlot.second.first.GetID()) {
                videoCamera->AddListener(m_videoListeners[1]);
                m_cameraSlot.second.second = videoCamera;
            }
            m_cameraIDToVideoCameraMap.insert(std::make_pair(camera.GetID(), videoCamera));
            m_videoCameraToCameraMap.insert(std::make_pair(videoCamera, camera));
        }
        for (auto& camera : m_entranceVideoCameras)
            std::thread([&]() { camera->ReadFrames(); }).detach();
        for (auto& camera : m_exitVideoCameras)
            std::thread([&]() { camera->ReadFrames(); }).detach();
        for (auto& camera : m_parkingVideoCameras) {
            std::thread([&]() { camera->ReadFrames(); }).detach();
            std::thread([&]() {
               
                ActionManagement dedicatedActionManagement(true);
                while (camera->IsCameraOpened()) {
                    try {
                    dedicatedActionManagement.StartAction(camera->GetCurrentFrame(), m_videoCameraToCameraMap[camera]);
                    }
                    catch (...) { std::cout << "\n\nskipped current frame due to unexpected error.\n\n"; }
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                } 
            }).detach();
        }

    }
    catch (const std::exception& exception) {
        Close();
        std::string exception_message = "\nGo to Camera Management and review your video cameras to continue using the application!";
        throw std::runtime_error(exception.what() + exception_message);
    }
}

void MainWindowController::ChangeCameraOnSlot(const DatabaseEntity::Camera& camera, bool isSlotOne)
{
    if (isSlotOne) {
        m_cameraSlot.first.second->RemoveListener(m_videoListeners[0]);
        m_cameraSlot.first = std::make_pair(camera, m_cameraIDToVideoCameraMap[camera.GetID()]);
        m_cameraSlot.first.second->AddListener(m_videoListeners[0]);
    }
    else {
        m_cameraSlot.second.second->RemoveListener(m_videoListeners[1]);
        m_cameraSlot.second = std::make_pair(camera, m_cameraIDToVideoCameraMap[camera.GetID()]);
        m_cameraSlot.second.second->AddListener(m_videoListeners[1]);
    }
}

void MainWindowController::ChangeDetectionTypeForCameraType(const DatabaseEntity::CameraType& cameraType, const QString& selectedDetectionType)
{
    try {
        JsonFileUtils::UpdateDetectionTypeInFile(cameraType.GetType(), selectedDetectionType);
    } catch (...) {
        throw std::runtime_error("An error occured while saving the new detection type to the json file! Try deleting detections.json or try again!");
    }
}

void MainWindowController::ForceExitAction(const DatabaseEntity::Camera& camera, const DatabaseEntity::Session& session)
{
    m_database.ToSession().ForceExitForSessionID(session.GetID());
}

void MainWindowController::ForcePhotoAction(const DatabaseEntity::Camera& camera, const QString& photoPath)
{
    cv::Mat inputFrame = cv::imread(cv::String(photoPath.toStdString()));

    m_actionManagement.StartAction(inputFrame, camera);
}

void MainWindowController::Refresh()
{
    try {
        Close();
        SetupCameras();
    }
    catch (const std::exception& exception) {
        throw exception;
    }
}

void MainWindowController::Close()
{
    if (m_cameraSlot.first.second != nullptr && m_cameraSlot.second.second != nullptr) {
        m_cameraSlot.first.second->RemoveListener(m_videoListeners[0]);
        m_cameraSlot.second.second->RemoveListener(m_videoListeners[1]);
    }
    for (size_t index = 0; index < m_entranceVideoCameras.size(); index++) {
        m_entranceVideoCameras[index]->StopCamera();
        //delete m_entranceVideoCameras[index];
    }
    for (size_t index = 0; index < m_exitVideoCameras.size(); index++) {
        m_exitVideoCameras[index]->StopCamera();
        //delete m_exitVideoCameras[index];
    }
    for (size_t index = 0; index < m_parkingVideoCameras.size(); index++) {
        m_parkingVideoCameras[index]->StopCamera();
        //delete m_parkingVideoCameras[index];
    }
    m_entranceVideoCameras.clear();
    m_exitVideoCameras.clear();
    m_parkingVideoCameras.clear();
    m_cameraIDToVideoCameraMap.clear();
    m_videoCameraToCameraMap.clear();
    std::this_thread::sleep_for(std::chrono::seconds(4));
}

void MainWindowController::GetDefaultCameras()
{
    JsonFile defaultCameras = JsonFileUtils::GetDefaultCamerasJsonFile();
    std::vector<QString> cameraOne = defaultCameras.ReadFromFile(JsonFileUtils::DEFAULT_SLOT_ONE_CELL);
    std::vector<QString> cameraTwo = defaultCameras.ReadFromFile(JsonFileUtils::DEFAULT_SLOT_TWO_CELL);
    if (cameraOne.size() < 2) {
        defaultCameras.RemoveElement(JsonFileUtils::DEFAULT_SLOT_ONE_CELL);
        defaultCameras.WriteToFile(std::vector<QString>{"1"}, JsonFileUtils::DEFAULT_SLOT_ONE_CELL);
        cameraOne = defaultCameras.ReadFromFile(JsonFileUtils::DEFAULT_SLOT_ONE_CELL);
    }
    if (cameraTwo.size() < 2) {
        defaultCameras.RemoveElement(JsonFileUtils::DEFAULT_SLOT_TWO_CELL);
        defaultCameras.WriteToFile(std::vector<QString>{"2"}, JsonFileUtils::DEFAULT_SLOT_TWO_CELL);
        cameraTwo = defaultCameras.ReadFromFile(JsonFileUtils::DEFAULT_SLOT_TWO_CELL);
    }
    std::pair < DatabaseEntity::Camera, VideoCamera*> firstSlot = std::make_pair(m_database.ToCamera().FindByID(cameraOne[1].toInt()), nullptr);
    std::pair<DatabaseEntity::Camera, VideoCamera*> secondSlot = std::make_pair(m_database.ToCamera().FindByID(cameraTwo[1].toInt()), nullptr);
    m_cameraSlot = std::make_pair(firstSlot, secondSlot);
}

MainWindowController::~MainWindowController()
{
    delete m_cameraManagementWindow;
   // delete m_exitVideoCameras;
    //delete m_entranceVideoCameras;
}