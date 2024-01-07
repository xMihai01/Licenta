#ifndef CAMERAMANAGEMENTWINDOW_H
#define CAMERAMANAGEMENTWINDOW_H

#include <QWidget>

namespace Ui {
class CameraManagementWindow;
}

class CameraManagementWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CameraManagementWindow(QWidget *parent = nullptr);
    ~CameraManagementWindow();

private:
    Ui::CameraManagementWindow *ui;
};

#endif // CAMERAMANAGEMENTWINDOW_H
