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
    
    (type == CustomComboBoxDialogType::PHOTO_ACTION ? layout->addWidget(pathLabel)
        : (type == CustomComboBoxDialogType::SLOT_SELECTION ? layout->addWidget(slotLabel) : layout->addWidget(sessionLabel))); 
    type == CustomComboBoxDialogType::PHOTO_ACTION ? layout->addWidget(lineEdit) : layout->addWidget(secondComboBox);
    layout->addWidget(cameraLabel);
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

    delete layout;
}

QString CustomComboBoxDialog::GetSecondComboBoxText()
{
    return secondComboBox->currentText();
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

void CustomComboBoxDialog::onOkButtonClicked() {
    accept();
}