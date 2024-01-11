#ifndef CAMERACOMBOBOXDIALOG_H
#define CAMERACOMBOBOXDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHash>

#include <Model/Database/Database.h>

#include <Model/Utils/QtKeysEnum.h>

class CameraComboBoxDialog : public QDialog {
    Q_OBJECT

public:

    enum class CameraComboBoxDialogType {
        SLOT_SELECTION
    };

public:
    CameraComboBoxDialog(const CameraComboBoxDialogType type, QWidget* parent = nullptr);
    ~CameraComboBoxDialog();

    DatabaseEntity::Camera GetChosenCamera();
    QtKeyEnum GetChosenKey();
    QString GetSecondComboBoxText();

private:

    void GetComboBoxMapForCameras();

private:
    QComboBox* cameraComboBox;
    QComboBox* secondComboBox;
    QPushButton* okButton;
    QVBoxLayout* layout;

    QHash<QString, DatabaseEntity::Camera> map;

private slots:
    void onOkButtonClicked();
};

#endif