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
        ONGOING_SESSION_SELECTION
    };

public:
    CustomComboBoxDialog(const CustomComboBoxDialogType type, QWidget* parent = nullptr);
    ~CustomComboBoxDialog();

    DatabaseEntity::Camera GetChosenCamera();
    DatabaseEntity::Session GetChosenSession();
    QString GetSecondComboBoxText();
    QString GetLineEditText();

private:

    void GetComboBoxMapForCameras();
    void GetComboBoxMapForOngoingSessions();
    void GetComboBoxMapForExitCameras();

private:
    QComboBox* cameraComboBox;
    QComboBox* secondComboBox;
    QLineEdit* lineEdit;
    QPushButton* okButton;

    QLabel* sessionLabel;
    QLabel* cameraLabel;
    QLabel* slotLabel;
    QLabel* pathLabel;

    QVBoxLayout* layout;

    QHash<QString, DatabaseEntity::Camera> map;
    QHash<QString, DatabaseEntity::Session> sessionsMap;

    CustomComboBoxDialogType m_type;

    CustomComboBoxDialogController m_dialogController;

private slots:
    void onOkButtonClicked();
};

#endif