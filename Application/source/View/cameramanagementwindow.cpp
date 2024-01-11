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
    ui->cameraLocationLineEdit->setReadOnly(mode == CameraManagementWindowController::CameraManagementMode::REMOVE || mode == CameraManagementWindowController::CameraManagementMode::LIST);
    ui->cameraNameLineEdit->setReadOnly(mode == CameraManagementWindowController::CameraManagementMode::REMOVE || mode == CameraManagementWindowController::CameraManagementMode::LIST);
    ui->cameraTypeComboBox->setEnabled(!(mode == CameraManagementWindowController::CameraManagementMode::REMOVE || mode == CameraManagementWindowController::CameraManagementMode::LIST));
    ui->cameraKeyComboBox->setEnabled(mode == CameraManagementWindowController::CameraManagementMode::UPDATE);
    ui->actionButton->setVisible(mode != CameraManagementWindowController::CameraManagementMode::LIST);
}

void CameraManagementWindow::OnActionButtonClick()
{
    try {
        if (ui->cameraLocationLineEdit->text() == "" && ui->cameraNameLineEdit->text() == "")
            throw std::runtime_error("All fields are required!");
        // If key is not available, throw exception. Two cameras can't share the same key! Should only happen during "ADD"
        if (m_windowController->GetCurrentMode() == CameraManagementWindowController::CameraManagementMode::UPDATE && m_windowController->GetDatabase().ToCameraKey().FindByKey(m_keysMap[ui->cameraKeyComboBox->currentText()]).GetID() != 0)
            throw std::runtime_error("Key is already in use!");
            
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

        if (m_windowController->GetCurrentMode() == CameraManagementWindowController::CameraManagementMode::UPDATE)
            m_windowController->AddOrUpdateKey(ui->cameraIDLineEdit->text().toInt(), m_keysMap[ui->cameraKeyComboBox->currentText()]);
        
        ReloadCameras();
        
        if (m_windowController->GetCurrentMode() == CameraManagementWindowController::CameraManagementMode::ADD)
            QMessageBox::about(this, "Warning", "Camera was added. Refresh cameras when going back to main window.\n\nKey must be set on the UPDATE window!");
        else
            QMessageBox::about(this, "Warning", "Cameras have been updated. Refresh cameras when going back to main window.");

    }
    catch (const std::exception& exception) {
        QMessageBox::critical(this, "Error", exception.what());
    }

}

void CameraManagementWindow::OnRunTestButtonClick()
{
    try {
        if (ui->cameraLocationLineEdit->text() == "") {
            QMessageBox::warning(this, "Warning!", "No path/index given!");
            return;
        }
            
        m_windowController->TestCamera(ui->cameraLocationLineEdit->text());
        QMessageBox::about(this, "Success!", "Camera test ran successfully!\n\nAfter exiting, a refresh might be needed to reload cameras correctly!");
    }
    catch (const std::exception& exception) {
        QMessageBox::critical(this, "Failed!", "Camera test failed!\n\nUsing this camera could make the application inoperable until removal!");
    }
}

void CameraManagementWindow::OnSlot1ButtonClick()
{
    try {
        m_windowController->UpdateDefaultCamera(m_camerasMap[m_selectedCamera], true);
        QMessageBox::about(this, "Warning", "Default Camera slot 1 has been updated. Refresh cameras when going back to main window.");
    }
    catch (const std::exception& exception) {
        QMessageBox::critical(this, "Error", exception.what());
    }
}

void CameraManagementWindow::OnSlot2ButtonClick()
{
    try {
        m_windowController->UpdateDefaultCamera(m_camerasMap[m_selectedCamera], false);
        QMessageBox::about(this, "Warning", "Default Camera slot 2 has been updated. Refresh cameras when going back to main window.");
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
        SetComboBoxForKeys();
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
    ui->cameraKeyComboBox->setCurrentIndex((static_cast<int>(m_windowController->GetDatabase().ToCameraKey().FindByID(m_camerasMap[camera].GetID()).GetKey()) - static_cast<int>(QtKeyEnum::A)));
}

void CameraManagementWindow::SetComboBoxForKeys()
{
    try {
        ui->cameraKeyComboBox->clear();
        for (uint32_t currentKey = static_cast<uint32_t>(QtKeyEnum::A); currentKey <= static_cast<uint32_t>(QtKeyEnum::Z); currentKey++) {
            QString comboBoxString = QString::fromStdString(QtEnumToString(QtKeyEnum(currentKey)))
                + (m_windowController->GetDatabase().ToCameraKey().IsKeyUsed(QtKeyEnum(currentKey)) ? " (USED)" : "");

            ui->cameraKeyComboBox->addItem(comboBoxString);
            m_keysMap[comboBoxString] = QtKeyEnum(currentKey);
        }
    }
    catch (const std::exception& exception) {
        throw exception;
    }
}