#include "Controller/MainWindowController.h"

MainWindowController::MainWindowController(QLabel* labelForEntranceCameraFrame, QLabel* labelForExitCameraFrame)
{
    std::thread([&]() {m_exitVideoCameras->OpenCamera("C:/Users/mihai/Desktop/Products/testVideo.mp4"); }).detach();

    std::thread([&]() {m_entranceVideoCameras->OpenCamera("C:/Users/mihai/Desktop/Products/testVideo2.mp4"); }).detach();

}

void MainWindowController::TakeEntranceFrame()
{
    static int index = 0;
    cv::imwrite("C:/Users/mihai/Desktop/Products/" + std::to_string(index) + "_entrance.jpg", m_entranceVideoCameras->GetCurrentFrame());
    index++;
}

void MainWindowController::TakeExitFrame()
{
    static int index2 = 0;
    cv::imwrite("C:/Users/mihai/Desktop/Products/" + std::to_string(index2) + "_exit.jpg", m_exitVideoCameras->GetCurrentFrame());
    index2++;
}

void MainWindowController::SetupCameras()
{

}

std::string MainWindowController::TakePlateFromFrame(const cv::Mat& frame)
{
    return std::string();
}
