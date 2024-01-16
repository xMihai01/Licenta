#include "View/parkingsetupwindow.h"
#include "ui_parkingsetupwindow.h"

ParkingSetupWindow::ParkingSetupWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParkingSetupWindow)
{
    ui->setupUi(this);

    m_label = new ClickableLabel();
    ui->gridLayout->addWidget(m_label);
    m_selectedCamera = nullptr;
    m_selectedSpace = nullptr;

    m_windowController = new ParkingSetupWindowController(m_label);

    connect(ui->parkingCamerasListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(OnCameraClick(QListWidgetItem*)));
    connect(ui->spacesListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(OnSpaceClick(QListWidgetItem*)));
    connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(OnResetButtonClick()));
    connect(ui->confirmButton, SIGNAL(clicked()), this, SLOT(OnConfirmButtonClick()));
    connect(ui->removeButton, SIGNAL(clicked()), this, SLOT(OnRemoveButtonClick()));
    connect(ui->updateButton, SIGNAL(clicked()), this, SLOT(OnUpdateButtonClick()));

    connect(m_label, SIGNAL(draw()), this, SLOT(OnDrawRequestFromClickableLabel()));
}

ParkingSetupWindow::~ParkingSetupWindow()
{
    delete m_windowController;
    delete ui;
}

void ParkingSetupWindow::OnCameraClick(QListWidgetItem* camera) {
    m_selectedCamera = camera;
    m_windowController->TakeSingleFrameFromCamera(m_parkingCamerasMap[camera]);
    ReloadSpaces();
}

void ParkingSetupWindow::ReloadCameras()
{
    try {
        m_parkingCamerasMap.clear();
        m_windowController->GetAllParkingCamerasInMap(m_parkingCamerasMap);
        ui->parkingCamerasListWidget->clear();
        for (const auto& camera : m_parkingCamerasMap.asKeyValueRange()) {
            ui->parkingCamerasListWidget->addItem(camera.first);
            camera.first->setText(QString::fromStdString(camera.second.GetName()));
        }
    }
    catch (const std::exception& exception) {
        QMessageBox::critical(this, "Error", exception.what());
    }
}

void ParkingSetupWindow::OnSpaceClick(QListWidgetItem* space)
{
    m_selectedSpace = space;
    m_windowController->ShowParkingSpace(m_parkingSpacesMap[space]);

}

void ParkingSetupWindow::OnResetButtonClick()
{
    m_windowController->Reset();
}

void ParkingSetupWindow::OnConfirmButtonClick()
{
    try {
        if (m_selectedCamera == nullptr)
            throw std::runtime_error("No parking camera selected!");
        bool passed;
        QString parkingSpaceName = QInputDialog::getText(this, tr("Enter parking space name"), tr("Parking space name"), QLineEdit::Normal, "A2");
        if (parkingSpaceName.isEmpty())
            throw std::runtime_error("Parking space name can not be empty!");

        m_windowController->AddParkingSpace(m_parkingCamerasMap[m_selectedCamera], m_label->GetFirstPoint(), m_label->GetSecondPoint(), parkingSpaceName);
        ReloadSpaces();
        QMessageBox::about(this, "Success", "Parking space added with name " + parkingSpaceName + "!");
    }
    catch (const std::exception& exception) {
        QMessageBox::critical(this, "Error", exception.what());
    }
}

void ParkingSetupWindow::OnUpdateButtonClick()
{
    try {
        if (m_selectedSpace == nullptr)
            throw std::runtime_error("No parking space selected to update!");
        bool passed;
        QString parkingSpaceName = QInputDialog::getText(this, tr("Enter new parking space name"), tr("New parking space name"), QLineEdit::Normal, "A2");
        if (parkingSpaceName.isEmpty())
            throw std::runtime_error("Parking space name can not be empty!");

        m_windowController->UpdateParkingSpace(m_parkingSpacesMap[m_selectedSpace], m_label->GetFirstPoint(), m_label->GetSecondPoint(), parkingSpaceName);
        ReloadSpaces();
        QMessageBox::about(this, "Success", "Parking space updated with name " + parkingSpaceName + "!");
    }
    catch (const std::exception& exception) {
        QMessageBox::critical(this, "Error", exception.what());
    }
}

void ParkingSetupWindow::OnRemoveButtonClick()
{
    try {
        if (m_selectedSpace == nullptr)
            throw std::runtime_error("No parking space selected to remove!");
   

        m_windowController->RemoveParkingSpace(m_parkingSpacesMap[m_selectedSpace]);
        ReloadSpaces();
        QMessageBox::about(this, "Success", "Parking space removed: " + QString::fromStdString(m_parkingSpacesMap[m_selectedSpace].GetName()) + "!");
    }
    catch (const std::exception& exception) {
        QMessageBox::critical(this, "Error", exception.what());
    }
}

void ParkingSetupWindow::OnDrawRequestFromClickableLabel()
{
    m_windowController->ShowParkingSpace(m_label->GetFirstPoint(), m_label->GetSecondPoint());
}

void ParkingSetupWindow::ReloadSpaces()
{
    try {
        m_parkingSpacesMap.clear();
        m_windowController->GetAllSpacesForParkingCamera(m_parkingSpacesMap, m_parkingCamerasMap[m_selectedCamera]);
        ui->spacesListWidget->clear();
        for (const auto& space : m_parkingSpacesMap.asKeyValueRange()) {
            ui->spacesListWidget->addItem(space.first);
            space.first->setText(QString::fromStdString(space.second.GetName()));
        }
    }
    catch (const std::exception& exception) {
        QMessageBox::critical(this, "Error", exception.what());
    }
}

