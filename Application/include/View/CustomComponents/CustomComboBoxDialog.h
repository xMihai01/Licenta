#ifndef CAMERACOMBOBOXDIALOG_H
#define CAMERACOMBOBOXDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QHash>

#include <Controller/CustomComboBoxDialogController.h>

#include <Model/Utils/QtKeysEnum.h>

class CustomComboBoxDialog : public QDialog {
    Q_OBJECT

public:

    enum class CustomComboBoxDialogType {
        SLOT_SELECTION,
        PHOTO_ACTION,
        ONGOING_SESSION_SELECTION,
        DETECTION_TYPE
    };

public:
    CustomComboBoxDialog(const CustomComboBoxDialogType type, QWidget* parent = nullptr);
    ~CustomComboBoxDialog();

    DatabaseEntity::Camera GetChosenCamera();
    DatabaseEntity::Session GetChosenSession();
    DatabaseEntity::CameraType GetChosenCameraType();
    QString GetSecondComboBoxText();
    QString GetFirstComboBoxText();
    QString GetLineEditText();

private:

    void GetComboBoxMapForCameras();
    void GetComboBoxMapForOngoingSessions();
    void GetComboBoxMapForExitCameras();
    void GetComboBoxMapForCameraType();

private:
    QComboBox* cameraComboBox;
    QComboBox* secondComboBox;
    QLineEdit* lineEdit;
    QPushButton* okButton;

    QLabel* sessionLabel;
    QLabel* cameraLabel;
    QLabel* slotLabel;
    QLabel* pathLabel;
    QLabel* cameraTypeLabel;
    QLabel* detectionTypeLabel;

    QVBoxLayout* layout;

    QHash<QString, DatabaseEntity::Camera> map;
    QHash<QString, DatabaseEntity::Session> sessionsMap;
    QHash<QString, DatabaseEntity::CameraType> cameraTypesMap;

    CustomComboBoxDialogType m_type;

    CustomComboBoxDialogController m_dialogController;

private slots:
    void onOkButtonClicked();
};

#endif