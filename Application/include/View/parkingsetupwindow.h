#ifndef PARKINGSETUPWINDOW_H
#define PARKINGSETUPWINDOW_H

#include <Controller/ParkingSetupWindowController.h>

#include <QWidget>
#include <QMessageBox>

namespace Ui {
class ParkingSetupWindow;
}

class ParkingSetupWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ParkingSetupWindow(QWidget *parent = nullptr);
    ~ParkingSetupWindow();

    void ReloadCameras();

private slots:

    void OnCameraClick(QListWidgetItem* camera);
    void OnResetButtonClick();
    void OnConfirmButtonClick();

    void OnDrawRequestFromClickableLabel();

private:
    Ui::ParkingSetupWindow *ui;

    QHash<QListWidgetItem*, DatabaseEntity::Camera> m_parkingCamerasMap;

    ClickableLabel* m_label;
    QListWidgetItem* m_selectedCamera;

    ParkingSetupWindowController* m_windowController;
};

#endif // PARKINGSETUPWINDOW_H
