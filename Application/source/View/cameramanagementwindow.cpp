#include "View/cameramanagementwindow.h"
#include "ui_cameramanagementwindow.h"

CameraManagementWindow::CameraManagementWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraManagementWindow)
{
    ui->setupUi(this);
    try {
        m_windowController = new CameraManagementWindowController();

        connect(ui->cameraWidgetList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(OnCameraClick(QListWidgetItem*)));
        connect(ui->actionButton, SIGNAL(clicked()), this, SLOT(OnActionButtonClick()));
        connect(ui->runTestButton, SIGNAL(clicked()), this, SLOT(OnRunTestButtonClick()));
        connect(ui->slot1Button, SIGNAL(clicked()), this, SLOT(OnSlot1ButtonClick()));
        connect(ui->slot2Button, SIGNAL(clicked()), this, SLOT(OnSlot2ButtonClick()));

        ReloadCameras();
        m_selectedCamera = nullptr;

    }
    catch (const std::exception& exception) {
        QMessageBox::critical(this, "Error", exception.what());
    }
    
}

CameraManagementWindow::~CameraManagementWindow()
{
    delete m_windowController;
    delete ui;
}

void CameraManagementWindow::ChangeMode(const CameraManagementWindowController::CameraManagementMode mode)
{
    switch (mode)
    {
    case CameraManagementWindowController::CameraManagementMode::ADD:
        ui->actionButton->setText("Add");
        break;
    case CameraManagementWindowController::CameraManagementMode::REMOVE:
        ui->actionButton->setText("Remove");
        break;
    case CameraManagementWindowController::CameraManagementMode::UPDATE:
        ui->actionButton->setText("Update");
        break;
    default:
        break;
    }
    m_windowController->SetCurrentMode(mode);
    ui->cameraLocationLineEdit->setReadOnly(mode == CameraManagementWindowController::CameraManagementMode::REMOVE);
    ui->cameraNameLineEdit->setReadOnly(mode == CameraManagementWindowController::CameraManagementMode::REMOVE);
    ui->cameraTypeComboBox->setEnabled(!(mode == CameraManagementWindowController::CameraManagementMode::REMOVE));
}

void CameraManagementWindow::OnActionButtonClick()
{
    try {
        if (ui->cameraLocationLineEdit->text() == "" && ui->cameraNameLineEdit->text() == "")
            throw std::runtime_error("All fields are required!");
        
        auto mode = m_windowController->GetCurrentMode();
        m_windowController->GetCurrentMode() == CameraManagementWindowController::CameraManagementMode::ADD

            ? m_windowController->DoAction(
                DatabaseEntity::Camera(
                    m_windowController->GetDatabase().ToCameraType().FindByType(m_windowController->GetDatabase().ToCameraType().ToBusinessLogic().ConvertQStringToType(ui->cameraTypeComboBox->currentText()))
                    , ui->cameraLocationLineEdit->text().toStdString()
                    , ui->cameraNameLineEdit->text().toStdString()))

            : m_windowController->DoAction(
                DatabaseEntity::Camera(
                    ui->cameraIDLineEdit->text().toInt()
                    , m_windowController->GetDatabase().ToCameraType().FindByType(m_windowController->GetDatabase().ToCameraType().ToBusinessLogic().ConvertQStringToType(ui->cameraTypeComboBox->currentText()))
                    , ui->cameraLocationLineEdit->text().toStdString()
                    , ui->cameraNameLineEdit->text().toStdString()));

        ReloadCameras();
        QMessageBox::warning(this, "Warning", "Cameras have been updated. Refresh cameras when going back to main window.");
    }
    catch (const std::exception& exception) {
        QMessageBox::critical(this, "Error", exception.what());
    }

}

void CameraManagementWindow::OnRunTestButtonClick()
{
    // TODO: Test how camera works during camera management
}

void CameraManagementWindow::OnSlot1ButtonClick()
{
    try {
        m_windowController->UpdateDefaultCamera(m_camerasMap[m_selectedCamera], true);
        QMessageBox::warning(this, "Warning", "Default Camera slot 1 has been updated. Refresh cameras when going back to main window.");
    }
    catch (const std::exception& exception) {
        QMessageBox::critical(this, "Error", exception.what());
    }
}

void CameraManagementWindow::OnSlot2ButtonClick()
{
    try {
        m_windowController->UpdateDefaultCamera(m_camerasMap[m_selectedCamera], false);
        QMessageBox::warning(this, "Warning", "Default Camera slot 2 has been updated. Refresh cameras when going back to main window.");
    }
    catch (const std::exception& exception) {
        QMessageBox::critical(this, "Error", exception.what());
    }
}

void CameraManagementWindow::ReloadCameras()
{
    try {
        m_camerasMap.clear();
        m_windowController->GetAllCamerasFromDatabaseInAMap(m_camerasMap);
        ui->cameraWidgetList->clear();
        for (const auto& camera : m_camerasMap.asKeyValueRange()) {
            ui->cameraWidgetList->addItem(camera.first);
            camera.first->setText(QString::fromStdString(camera.second.GetName()));
        }
    }
    catch (const std::exception& exception) {
        QMessageBox::critical(this, "Error", exception.what());
    }
}

void CameraManagementWindow::OnCameraClick(QListWidgetItem* camera) {
    m_selectedCamera = camera;
    ui->cameraIDLineEdit->setText(QString::number(m_camerasMap[camera].GetID()));
    ui->cameraLocationLineEdit->setText(QString::fromStdString(m_camerasMap[camera].GetLocation()));
    ui->cameraNameLineEdit->setText(QString::fromStdString(m_camerasMap[camera].GetName()));
    ui->cameraTypeComboBox->setCurrentText(m_windowController->GetDatabase().ToCameraType().ToBusinessLogic().ConvertTypeToQString(m_camerasMap[camera].GetCameraType().GetType()));
}
