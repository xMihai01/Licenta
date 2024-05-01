#include <View/CustomComponents/CustomComboBoxDialog.h>

CustomComboBoxDialog::CustomComboBoxDialog(const CustomComboBoxDialogType type, QWidget* parent) : QDialog(parent) , m_type(type)
{
    cameraComboBox = new QComboBox;
    secondComboBox = new QComboBox;
    lineEdit = new QLineEdit();
    okButton = new QPushButton("OK");
    layout = new QVBoxLayout(this);

    cameraLabel = new QLabel("Camera:");
    sessionLabel = new QLabel("Session:");
    slotLabel = new QLabel("Slot:");
    pathLabel = new QLabel("Photo path:");
    cameraTypeLabel = new QLabel("Camera Type:");
    detectionTypeLabel = new QLabel("Detection Type:");
    
    (type == CustomComboBoxDialogType::PHOTO_ACTION ? layout->addWidget(pathLabel)
        : (type == CustomComboBoxDialogType::SLOT_SELECTION ? layout->addWidget(slotLabel) 
            : (type == CustomComboBoxDialogType::ONGOING_SESSION_SELECTION ? layout->addWidget(sessionLabel) : layout->addWidget(cameraTypeLabel)))); 
    type == CustomComboBoxDialogType::PHOTO_ACTION ? layout->addWidget(lineEdit) : layout->addWidget(secondComboBox);
    type == CustomComboBoxDialogType::DETECTION_TYPE ? layout->addWidget(detectionTypeLabel) : layout->addWidget(cameraLabel);
    layout->addWidget(cameraComboBox);
    layout->addWidget(okButton);

    connect(okButton, SIGNAL(clicked()), this, SLOT(onOkButtonClicked()));

    switch (type)
    {
    case CustomComboBoxDialogType::SLOT_SELECTION:
        secondComboBox->addItem("Slot1");
        secondComboBox->addItem("Slot2");
        GetComboBoxMapForCameras();
        break;
    case CustomComboBoxDialogType::ONGOING_SESSION_SELECTION:
        GetComboBoxMapForExitCameras();
        GetComboBoxMapForOngoingSessions();
        break;
    case CustomComboBoxDialogType::PHOTO_ACTION:
        GetComboBoxMapForCameras();
        break;
    case CustomComboBoxDialogType::DETECTION_TYPE:
        cameraComboBox->addItem("IMAGE_PROCESSING");
        cameraComboBox->addItem("DNN");
        GetComboBoxMapForCameraType();
        break;
    default:
        break;
    }
}

CustomComboBoxDialog::~CustomComboBoxDialog()
{
    delete cameraComboBox;
    delete secondComboBox;
    delete lineEdit;
    delete okButton;

    delete cameraLabel;
    delete sessionLabel;
    delete pathLabel;
    delete slotLabel;
    delete cameraTypeLabel;
    delete detectionTypeLabel;

    delete layout;
}

QString CustomComboBoxDialog::GetSecondComboBoxText()
{
    return secondComboBox->currentText();
}

QString CustomComboBoxDialog::GetFirstComboBoxText()
{
    return cameraComboBox->currentText();
}

QString CustomComboBoxDialog::GetLineEditText()
{
    return lineEdit->text();
}

void CustomComboBoxDialog::GetComboBoxMapForCameras()
{
    m_dialogController.GetComboBoxMapForCameras(map);

    for (const auto& item : map.asKeyValueRange()) {
        cameraComboBox->addItem(item.first);
    }
}

void CustomComboBoxDialog::GetComboBoxMapForOngoingSessions()
{
    m_dialogController.GetComboBoxMapForOngoingSessions(sessionsMap);
    for (const auto& item : sessionsMap.asKeyValueRange())
        secondComboBox->addItem(item.first);
}

void CustomComboBoxDialog::GetComboBoxMapForExitCameras()
{
    m_dialogController.GetComboBoxMapForExitCameras(map);

    for (const auto& item : map.asKeyValueRange()) {
        cameraComboBox->addItem(item.first);
    }
}

void CustomComboBoxDialog::GetComboBoxMapForCameraType()
{
    m_dialogController.GetComboBoxMapForCameraType(cameraTypesMap);

    for (const auto& item : cameraTypesMap.asKeyValueRange()) {
        secondComboBox->addItem(item.first);
    }
}

DatabaseEntity::Camera CustomComboBoxDialog::GetChosenCamera()
{
    return map[cameraComboBox->currentText()];
}

DatabaseEntity::Session CustomComboBoxDialog::GetChosenSession()
{
    if (m_type == CustomComboBoxDialogType::SLOT_SELECTION)
        throw std::runtime_error("Couldn't get chosen session. Dialog was opened in SLOT_SELECTION mode, but it is not supported.");
    return sessionsMap[secondComboBox->currentText()];
}

DatabaseEntity::CameraType CustomComboBoxDialog::GetChosenCameraType()
{
    if (m_type != CustomComboBoxDialogType::DETECTION_TYPE)
        throw std::runtime_error("Couldn't get chosen camera type. Dialog was opened in DETECTION_TYPE mode, but it is not supported.");
    return cameraTypesMap[secondComboBox->currentText()];
}

void CustomComboBoxDialog::onOkButtonClicked() {
    accept();
}