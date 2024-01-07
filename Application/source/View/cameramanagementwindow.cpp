#include "cameramanagementwindow.h"
#include "ui_cameramanagementwindow.h"

CameraManagementWindow::CameraManagementWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraManagementWindow)
{
    ui->setupUi(this);
}

CameraManagementWindow::~CameraManagementWindow()
{
    delete ui;
}
