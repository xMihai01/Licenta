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
    connect(ui->actionChange_Detection_Type, SIGNAL(triggered()), this, SLOT(OnChangeDetectionTypeButtonClick()));
    connect(ui->systemTurnButton, SIGNAL(clicked()), this, SLOT(OnRefreshButtonClicked()));

    // Camera management buttons (add, remove, update etc)
    connect(ui->addCameraAction, SIGNAL(triggered()), this, SLOT(OnCameraManagementAddButtonClick()));
    connect(ui->removeCameraAction, SIGNAL(triggered()), this, SLOT(OnCameraManagementRemoveButtonClick()));
    connect(ui->updateCameraAction, SIGNAL(triggered()), this, SLOT(OnCameraManagementUpdateButtonClick()));
    connect(ui->manageParkingAction, SIGNAL(triggered()), this, SLOT(OnCameraManagementParkingButtonClick()));
   

    // Manage entries buttons
    connect(ui->forceExitForEntryAction, SIGNAL(triggered()), this, SLOT(OnForceExitForEntryButtonClicked()));
    connect(ui->forcePhotoEntryAction, SIGNAL(triggered()), this, SLOT(OnForceActionByPhotoButtonClicked()));

}

void MainWindow::GetFrame(const uint32_t cameraID)
{
    try {
        //qDebug() << cameraID;
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
    CustomComboBoxDialog dialog(CustomComboBoxDialog::CustomComboBoxDialogType::SLOT_SELECTION, this);
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

void MainWindow::OnChangeDetectionTypeButtonClick()
{
    if (repairMode) {
        QMessageBox::critical(this, "Error", "Can't use this while in repair mode!\n\nReview Camera Management and refresh!");
        return;
    }
    CustomComboBoxDialog dialog(CustomComboBoxDialog::CustomComboBoxDialogType::DETECTION_TYPE, this);
    if (dialog.exec() == QDialog::Accepted) {
        DatabaseEntity::CameraType chosenCameraType = dialog.GetChosenCameraType();
        windowController->ChangeDetectionTypeForCameraType(chosenCameraType, dialog.GetFirstComboBoxText());
        QMessageBox::information(this, "Success", "Detection type changed successfully!");
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
        repairMode = true;
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

void MainWindow::OnCameraManagementParkingButtonClick()
{
    if (repairMode) {
        QMessageBox::critical(this, "Error", "Can't use this while in repair mode!\n\nReview Camera Management and refresh!");
        return;
    }
    windowController->OpenParkingManagementWindow();
}

void MainWindow::OnForceExitForEntryButtonClicked()
{
    if (repairMode) {
        QMessageBox::critical(this, "Error", "Can't use this while in repair mode!\n\nReview Camera Management and refresh!");
        return;
    }
    CustomComboBoxDialog dialog(CustomComboBoxDialog::CustomComboBoxDialogType::ONGOING_SESSION_SELECTION, this);
    if (dialog.exec() == QDialog::Accepted) {
        DatabaseEntity::Camera chosenCamera = dialog.GetChosenCamera();
        DatabaseEntity::Session chosenSession = dialog.GetChosenSession();
        if (chosenSession.GetID() == 0) {
            QMessageBox::warning(this, "Warning", "No session selected. Skipping...");
            return;
        }
        windowController->ForceExitAction(chosenCamera, chosenSession);
    }
}

void MainWindow::OnForceActionByPhotoButtonClicked()
{
    if (repairMode) {
        QMessageBox::critical(this, "Error", "Can't use this while in repair mode!\n\nReview Camera Management and refresh!");
        return;
    }
    CustomComboBoxDialog dialog(CustomComboBoxDialog::CustomComboBoxDialogType::PHOTO_ACTION, this);
    if (dialog.exec() == QDialog::Accepted) {
        DatabaseEntity::Camera chosenCamera = dialog.GetChosenCamera();
        QString pathToPhoto = dialog.GetLineEditText();
        if (pathToPhoto == "") {
            QMessageBox::warning(this, "Warning", "No path for photo given. Skipping...");
            return;
        }
        try {
            windowController->ForcePhotoAction(chosenCamera, pathToPhoto);
        }
        catch (const std::exception& exception) {
            QMessageBox::critical(this, "Error", exception.what());
        }
    }
}

void MainWindow::ReloadKeys()
{
    for (auto& connection : m_keyConnections)
        QObject::disconnect(connection);
    for (auto& shortcut : m_keyShortcuts)
        delete shortcut;
    m_keyConnections.clear();
    m_keyShortcuts.clear();

    DatabaseBusinessLogic::CameraKey cameraKeyBusinessLogic = DatabaseBusinessLogic::CameraKey();
    std::vector<DatabaseEntity::CameraKey> allKeys = cameraKeyBusinessLogic.FindAll();

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

