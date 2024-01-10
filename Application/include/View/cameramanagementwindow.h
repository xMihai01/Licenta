#ifndef CAMERAMANAGEMENTWINDOW_H
#define CAMERAMANAGEMENTWINDOW_H

#include <QWidget>

#include <Controller/CameraManagementWindowController.h>

namespace Ui {
class CameraManagementWindow;
}

class CameraManagementWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CameraManagementWindow(QWidget *parent = nullptr);
    ~CameraManagementWindow();

    void ChangeMode(const CameraManagementWindowController::CameraManagementMode mode);

private slots:

    void OnCameraClick(QListWidgetItem* camera);
    void OnActionButtonClick();
    void OnRunTestButtonClick();

    void OnSlot1ButtonClick();
    void OnSlot2ButtonClick();

private:

    void ReloadCameras();

private:
    Ui::CameraManagementWindow *ui;

    QHash<QListWidgetItem*, DatabaseEntity::Camera> m_camerasMap;

    QListWidgetItem* m_selectedCamera;

    CameraManagementWindowController* m_windowController;
};

#endif // CAMERAMANAGEMENTWINDOW_H
