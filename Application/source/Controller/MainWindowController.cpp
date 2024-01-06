#include "Controller/MainWindowController.h"

MainWindowController::MainWindowController(QLabel* labelForEntranceCameraFrame, QLabel* labelForExitCameraFrame)
    : m_labelForEntranceCameraFrame(labelForEntranceCameraFrame)
    , m_labelForExitCameraFrame(labelForExitCameraFrame)
{
    try {
        // currently hardcoded \/
        m_videoListeners.push_back(std::make_shared<InterfaceVideoListener>(labelForEntranceCameraFrame));
        m_videoListeners.push_back(std::make_shared<InterfaceVideoListener>(labelForExitCameraFrame));
        m_entranceVideoCameras = new VideoCamera();
        m_exitVideoCameras = new VideoCamera();
        m_entranceVideoCameras->AddListener(m_videoListeners[0]);
        m_exitVideoCameras->AddListener(m_videoListeners[1]);

        m_exitVideoCameras->OpenCamera("C:/Users/mihai/Desktop/Products/testVideo.mp4");
        m_entranceVideoCameras->OpenCamera(0);
        std::thread([&]() { m_exitVideoCameras->ReadFrames(); }).detach();

        std::thread([&]() { m_entranceVideoCameras->ReadFrames(); }).detach();
        // /\
       
        m_database.Connect("main");
       
    }
    catch (const std::exception& exception) {
        throw exception;
    }
    
}

void MainWindowController::TakeEntranceFrame()
{
    cv::Mat frame = m_entranceVideoCameras->GetCurrentFrame();
    static int index = 0;
    cv::imwrite("C:/Users/mihai/Desktop/Products/img/" + std::to_string(index) + "_entrance.jpg", frame);
    index++;
}

void MainWindowController::TakeExitFrame()
{
    cv::Mat frame = m_exitVideoCameras->GetCurrentFrame();
    static int index2 = 0;
    cv::imwrite("C:/Users/mihai/Desktop/Products/img/" + std::to_string(index2) + "_exit.jpg", frame);
    index2++;
}

void MainWindowController::SetupCameras()
{

}

void MainWindowController::Close()
{
    m_entranceVideoCameras->RemoveListener(m_videoListeners[0]);
    m_exitVideoCameras->RemoveListener(m_videoListeners[1]);
    m_entranceVideoCameras->StopCamera();   
    m_exitVideoCameras->StopCamera();
}

std::string MainWindowController::TakePlateFromFrame(const cv::Mat& frame)
{
    return std::string();
}

MainWindowController::~MainWindowController()
{
   // delete m_exitVideoCameras;
    //delete m_entranceVideoCameras;
}