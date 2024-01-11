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

    // Gets all keys for each camera
    ReloadKeys();

    // Button for changing video source for parking lot cameras
    connect(ui->changeVideoSourceAction, SIGNAL(triggered()), this, SLOT(OnChangeVideoSourceButtonClick()));
    connect(ui->viewSpecificCameraAction, SIGNAL(triggered()), this, SLOT(OnViewSpecificCameraButtonClick()));
    connect(ui->setSpecificKeyForCameraAction, SIGNAL(triggered()), this, SLOT(OnSetSpecificKeyForCameraButtonClick()));
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

void MainWindow::GetFrame(const uint32_t cameraID)
{
    try {
        qDebug() << cameraID;
    }
    catch (const std::exception& exception) {
        QMessageBox::critical(this, "Error", exception.what());
    }
}

void MainWindow::OnViewSpecificCameraButtonClick()
{
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

void MainWindow::OnSetSpecificKeyForCameraButtonClick()
{
    try {
        CameraComboBoxDialog dialog(CameraComboBoxDialog::CameraComboBoxDialogType::KEY_SELECTION, this);
        if (dialog.exec() == QDialog::Accepted) {
            DatabaseEntity::Camera chosenCamera = dialog.GetChosenCamera();
            windowController->ChangeCameraKey(chosenCamera, QtKeyEnum(dialog.GetChosenKey()));
            QMessageBox::about(this, "Success!", "Key changed successfully!");
            ReloadKeys();
        }
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
        windowController->Refresh();
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
    windowController->Close();
    event->accept();
}

MainWindow::~MainWindow()
{
    delete windowController;
    delete ui;
}

