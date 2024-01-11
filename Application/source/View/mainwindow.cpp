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
        repairMode = true;
        repairWindow = new CameraManagementWindow();
    }

    // Gets all keys for each camera
    ReloadKeys();

    // Button for changing video source for parking lot cameras
    connect(ui->viewSpecificCameraAction, SIGNAL(triggered()), this, SLOT(OnViewSpecificCameraButtonClick()));
    connect(ui->systemTurnButton, SIGNAL(clicked()), this, SLOT(OnRefreshButtonClicked()));

    // Camera management buttons (add, remove, update etc(
    connect(ui->addCameraAction, SIGNAL(triggered()), this, SLOT(OnCameraManagementAddButtonClick()));
    connect(ui->removeCameraAction, SIGNAL(triggered()), this, SLOT(OnCameraManagementRemoveButtonClick()));
    connect(ui->updateCameraAction, SIGNAL(triggered()), this, SLOT(OnCameraManagementUpdateButtonClick()));

}

void MainWindow::GetFrame(const uint32_t cameraID)
{
    try {
        qDebug() << cameraID;
        windowController->GetFrameAndStartAction(cameraID);
    }
    catch (const std::exception& exception) {
        QMessageBox::critical(this, "Error", exception.what());
    }
}

void MainWindow::OnViewSpecificCameraButtonClick()
{
    if (repairMode) {
        QMessageBox::critical(this, "Error", "Can't use this while in repair mode!\n\nReview Camera Management and refresh!");
        return;
    }
    CameraComboBoxDialog dialog(CameraComboBoxDialog::CameraComboBoxDialogType::SLOT_SELECTION, this);
    if (dialog.exec() == QDialog::Accepted) {
        DatabaseEntity::Camera chosenCamera = dialog.GetChosenCamera();
        if (dialog.GetSecondComboBoxText() == "Slot1") {
            windowController->ChangeCameraOnSlot(chosenCamera, true);
        }
        else {
            windowController->ChangeCameraOnSlot(chosenCamera, false);
        }
    }
}

void MainWindow::OnRefreshButtonClicked()
{
    try {
        if (repairMode) {
            windowController = new MainWindowController(ui->entranceCameraLabel, ui->exitCameraLabel);
        }
        windowController->Refresh();
        ReloadKeys();
        repairMode = false;
    }
    catch (const std::exception& exception) {
        QMessageBox::critical(this, "Error", exception.what());
    }
}

void MainWindow::OnCameraManagementAddButtonClick()
{
    if (repairMode) {
        repairWindow->ChangeMode(CameraManagementWindowController::CameraManagementMode::ADD);
        repairWindow->show();
        return;
    }
    windowController->OpenCameraManagementWindow(CameraManagementWindowController::CameraManagementMode::ADD);
}

void MainWindow::OnCameraManagementRemoveButtonClick()
{
    if (repairMode) {
        repairWindow->ChangeMode(CameraManagementWindowController::CameraManagementMode::REMOVE);
        repairWindow->show();
        return;
    }
    windowController->OpenCameraManagementWindow(CameraManagementWindowController::CameraManagementMode::REMOVE);
}

void MainWindow::OnCameraManagementUpdateButtonClick()
{
    if (repairMode) {
        repairWindow->ChangeMode(CameraManagementWindowController::CameraManagementMode::UPDATE);
        repairWindow->show();
        return;
    }
    windowController->OpenCameraManagementWindow(CameraManagementWindowController::CameraManagementMode::UPDATE);
}

void MainWindow::ReloadKeys()
{
    for (auto& connection : m_keyConnections)
        QObject::disconnect(connection);
    for (auto& shortcut : m_keyShortcuts)
        delete shortcut;
    m_keyConnections.clear();
    m_keyShortcuts.clear();

    DatabaseDataAccess::CameraKey cameraKeyDataAccess;
    std::vector<DatabaseEntity::CameraKey> allKeys = cameraKeyDataAccess.FindAll();

    for (auto& key : allKeys) {
        const uint32_t cameraIDForKey = key.GetID();

        QShortcut* shortcut = new QShortcut(QKeySequence(static_cast<int>(key.GetKey())), this);
        m_keyShortcuts.push_back(shortcut);

        QMetaObject::Connection connection = connect(shortcut, &QShortcut::activated, this, [cameraIDForKey, this](){GetFrame(cameraIDForKey);});
        m_keyConnections.push_back(connection);
    }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (!repairMode)
        windowController->Close();
    event->accept();
}

MainWindow::~MainWindow()
{
    delete windowController;
    delete ui;
}

