#include <View/CustomComponents/CameraComboBoxDialog.h>

CameraComboBoxDialog::CameraComboBoxDialog(const CameraComboBoxDialogType type, QWidget* parent) : QDialog(parent) {
    cameraComboBox = new QComboBox;
    secondComboBox = new QComboBox;
    okButton = new QPushButton("OK");
    layout = new QVBoxLayout(this);

    layout->addWidget(secondComboBox);
    layout->addWidget(cameraComboBox);
    layout->addWidget(okButton);

    connect(okButton, SIGNAL(clicked()), this, SLOT(onOkButtonClicked()));

    switch (type)
    {
    case CameraComboBoxDialogType::SLOT_SELECTION:
        secondComboBox->addItem("Slot1");
        secondComboBox->addItem("Slot2");
        GetComboBoxMapForCameras();
        break;
    case CameraComboBoxDialogType::KEY_SELECTION:
        GetComboBoxMapForCameras();
        SetComboBoxForKeys();
        break;
    default:
        break;
    }
}

CameraComboBoxDialog::~CameraComboBoxDialog()
{
    delete cameraComboBox;
    delete okButton;
    delete layout;
}

QString CameraComboBoxDialog::GetSecondComboBoxText()
{
    return secondComboBox->currentText();
}

void CameraComboBoxDialog::GetComboBoxMapForCameras()
{
    Database database;
    std::vector<DatabaseEntity::Camera> cameras = database.ToCamera().FindAll();

    for (auto& camera : cameras) {
        int id = camera.GetID();
        QString comboBoxString = QString::fromStdString(camera.GetName()) + " (" + QString::number(id) + ")";
        cameraComboBox->addItem(comboBoxString);
        map[comboBoxString] = camera;
    }
}

void CameraComboBoxDialog::SetComboBoxForKeys()
{
    for (uint32_t currentKey = static_cast<uint32_t>(QtKeyEnum::A); currentKey <= static_cast<uint32_t>(QtKeyEnum::Z); currentKey++) {
        QString comboBoxString = QString::fromStdString(QtEnumToString(QtKeyEnum(currentKey)));
        secondComboBox->addItem(comboBoxString);
        keysMap[comboBoxString] = QtKeyEnum(currentKey);
    }
}

DatabaseEntity::Camera CameraComboBoxDialog::GetChosenCamera()
{
    return map[cameraComboBox->currentText()];
}

QtKeyEnum CameraComboBoxDialog::GetChosenKey()
{
    return keysMap[secondComboBox->currentText()];
}

void CameraComboBoxDialog::onOkButtonClicked() {
    accept();
}