#include "View/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    try {
        this->windowController = new MainWindowController(ui->entranceCameraLabel, ui->exitCameraLabel);
    }
    catch (const std::exception& exception) {
        QMessageBox::critical(this, "Error", exception.what());
    }

    // For taking a photo from video at the entrance, by pressing key E, or at the exit, by pressing key Q
    connect(new QShortcut(QKeySequence(Qt::Key_E), this), &QShortcut::activated, this, &MainWindow::GetEntranceFrame);
    connect(new QShortcut(QKeySequence(Qt::Key_Q), this), &QShortcut::activated, this, &MainWindow::GetExitFrame);

    // Button for changing video source for parking lot cameras
    connect(ui->changeVideoSourceAction, SIGNAL(triggered()), this, SLOT(OnChangeVideoSourceButtonClick()));
    connect(ui->systemTurnButton, SIGNAL(clicked()), this, SLOT(OnRefreshButtonClicked()));

    // Camera management buttons (add, remove, update etc(
    connect(ui->addCameraAction, SIGNAL(triggered()), this, SLOT(OnCameraManagementAddButtonClick()));
    connect(ui->removeCameraAction, SIGNAL(triggered()), this, SLOT(OnCameraManagementRemoveButtonClick()));
    connect(ui->updateCameraAction, SIGNAL(triggered()), this, SLOT(OnCameraManagementUpdateButtonClick()));

}

void MainWindow::GetEntranceFrame() {
    try {
        windowController->TakeEntranceFrame();
    }
    catch (const std::exception& exception) {
        QMessageBox::critical(this, "Error", exception.what());
    }
}

void MainWindow::GetExitFrame() {
    try {
        windowController->TakeExitFrame();
    }
    catch (const std::exception& exception) {
        QMessageBox::critical(this, "Error", exception.what());
    }
}

void MainWindow::OnChangeVideoSourceButtonClick() {
    
    // TODO: Allow user to change video sources
    qDebug() << "button press";
}

void MainWindow::OnRefreshButtonClicked()
{
    try {
        windowController->SetupCameras();
    }
    catch (const std::exception& exception) {
        QMessageBox::critical(this, "Error", exception.what());
    }
}

void MainWindow::OnCameraManagementAddButtonClick()
{
    windowController->OpenCameraManagementWindow(CameraManagementWindowController::CameraManagementMode::ADD);
}

void MainWindow::OnCameraManagementRemoveButtonClick()
{
    windowController->OpenCameraManagementWindow(CameraManagementWindowController::CameraManagementMode::REMOVE);
}

void MainWindow::OnCameraManagementUpdateButtonClick()
{
    windowController->OpenCameraManagementWindow(CameraManagementWindowController::CameraManagementMode::UPDATE);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    windowController->Close();
    event->accept();
}

MainWindow::~MainWindow()
{
    delete windowController;
    delete ui;
}

