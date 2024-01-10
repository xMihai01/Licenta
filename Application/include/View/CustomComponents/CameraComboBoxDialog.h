#ifndef CAMERACOMBOBOXDIALOG_H
#define CAMERACOMBOBOXDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHash>

#include <Model/Database/Database.h>

class CameraComboBoxDialog : public QDialog {
    Q_OBJECT

public:
    CameraComboBoxDialog(QWidget* parent = nullptr);
    ~CameraComboBoxDialog();

    DatabaseEntity::Camera GetChosenCamera();
    QString GetSlotComboBoxText();

private:

    void GetComboBoxMapForCameras();

private:
    QComboBox* cameraComboBox;
    QComboBox* slotComboBox;
    QPushButton* okButton;
    QVBoxLayout* layout;

    QHash<QString, DatabaseEntity::Camera> map;

private slots:
    void onOkButtonClicked();
};

#endif