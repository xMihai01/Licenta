#include "Controller/MainWindowController.h"

MainWindowController::MainWindowController(QLabel* labelForEntranceCameraFrame, QLabel* labelForExitCameraFrame)
    : m_labelForEntranceCameraFrame(labelForEntranceCameraFrame)
    , m_labelForExitCameraFrame(labelForExitCameraFrame)
{
    try {
        m_database.Connect("main");

        // currently hardcoded \/
        m_videoListeners.push_back(std::make_shared<InterfaceVideoListener>(labelForEntranceCameraFrame));
        m_videoListeners.push_back(std::make_shared<InterfaceVideoListener>(labelForExitCameraFrame));
        //m_entranceVideoCameras = new VideoCamera();
        //m_exitVideoCameras = new VideoCamera();
        //m_entranceVideoCameras->AddListener(m_videoListeners[0]);
        //m_exitVideoCameras->AddListener(m_videoListeners[1]);

        //m_exitVideoCameras->OpenCamera("C:/Users/mihai/Desktop/Products/testVideo3.mp4");
        //m_entranceVideoCameras->OpenCamera("C:/Users/mihai/Desktop/Products/testVideo3.mp4");
        //std::thread([&]() { m_exitVideoCameras->ReadFrames(); }).detach();

        //std::thread([&]() { m_entranceVideoCameras->ReadFrames(); }).detach();
        // /\
        
        SetupCameras();
        
        m_cameraManagementWindow = new CameraManagementWindow();
    }
    catch (const std::exception& exception) {
        throw exception;
    }
    
}

void MainWindowController::OpenCameraManagementWindow(const CameraManagementWindowController::CameraManagementMode mode)
{
    if (m_cameraManagementWindow->isVisible())
        m_cameraManagementWindow->hide();
    m_cameraManagementWindow->ChangeMode(mode);
    m_cameraManagementWindow->show();
}

void MainWindowController::ChangeCameraKey(const DatabaseEntity::Camera& camera, const QtKeyEnum key)
{
    if (m_database.ToCameraKey().FindByID(camera.GetID()).GetID() != 0)
        m_database.ToCameraKey().Update(DatabaseEntity::CameraKey(camera.GetID(), key));
    else
        m_database.ToCameraKey().Add(DatabaseEntity::CameraKey(camera.GetID(), key));
}

void MainWindowController::TakeEntranceFrame()
{
    //cv::Mat frame = m_entranceVideoCameras->GetCurrentFrame();
    static int index = 0;
    //cv::imwrite("C:/Users/mihai/Desktop/Products/img/" + std::to_string(index) + "_entrance.jpg", frame);
    index++;
}

void MainWindowController::TakeExitFrame()
{
    //cv::Mat frame = m_exitVideoCameras->GetCurrentFrame();
    static int index2 = 0;
    //cv::imwrite("C:/Users/mihai/Desktop/Products/img/" + std::to_string(index2) + "_exit.jpg", frame);
    index2++;
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
        }
        for (auto& camera : m_entranceVideoCameras)
            std::thread([&]() { camera->ReadFrames(); }).detach();
        for (auto& camera : m_exitVideoCameras)
            std::thread([&]() { camera->ReadFrames(); }).detach();
        for (auto& camera : m_parkingVideoCameras)
            std::thread([&]() { camera->ReadFrames(); }).detach();
    }
    catch (const std::exception& exception) {
        throw exception;
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
}

std::string MainWindowController::TakePlateFromFrame(const cv::Mat& frame)
{
    return std::string();
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