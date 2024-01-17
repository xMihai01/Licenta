#ifndef PARKINGSETUPWINDOW_H
#define PARKINGSETUPWINDOW_H

#include <Controller/ParkingSetupWindowController.h>

#include <QWidget>
#include <QMessageBox>
#include <QInputDialog>

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
    void OnSpaceClick(QListWidgetItem* space);
    void OnResetButtonClick();
    void OnConfirmButtonClick();
    void OnUpdateButtonClick();
    void OnRemoveButtonClick();

    void OnDrawRequestFromClickableLabel();

private:

    void ReloadSpaces();
    void closeEvent(QCloseEvent* event);

private:
    Ui::ParkingSetupWindow *ui;

    QHash<QListWidgetItem*, DatabaseEntity::Camera> m_parkingCamerasMap;
    QHash<QListWidgetItem*, DatabaseEntity::ParkingSpace> m_parkingSpacesMap;

    ClickableLabel* m_label;
    QListWidgetItem* m_selectedCamera;
    QListWidgetItem* m_selectedSpace;

    ParkingSetupWindowController* m_windowController;
};

#endif // PARKINGSETUPWINDOW_H
