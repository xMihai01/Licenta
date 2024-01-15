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

    m_windowController = new ParkingSetupWindowController(m_label);

    connect(ui->parkingCamerasListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(OnCameraClick(QListWidgetItem*)));
    connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(OnResetButtonClick()));
    connect(ui->confirmButton, SIGNAL(clicked()), this, SLOT(OnConfirmButtonClick()));

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
}

void ParkingSetupWindow::ReloadCameras()
{
    try {
        // TODO: potential memory leak caused by m_windowController->GetAllParkingCamerasInMap(m_parkingCamerasMap). A new QListWidgetItem is allocated every time.
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

void ParkingSetupWindow::OnResetButtonClick()
{
    m_windowController->Reset();
}

void ParkingSetupWindow::OnConfirmButtonClick()
{
    try {
        m_windowController->ShowParkingSpace(m_label->GetFirstPoint(), m_label->GetSecondPoint());

    }
    catch (const std::exception& exception) {
        QMessageBox::critical(this, "Error", exception.what());
    }
}

void ParkingSetupWindow::OnDrawRequestFromClickableLabel()
{
    m_windowController->ShowParkingSpace(m_label->GetFirstPoint(), m_label->GetSecondPoint());
}
