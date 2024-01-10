#include <View/CustomComponents/CameraComboBoxDialog.h>

CameraComboBoxDialog::CameraComboBoxDialog(QWidget* parent) : QDialog(parent) {
    cameraComboBox = new QComboBox;
    slotComboBox = new QComboBox;
    okButton = new QPushButton("OK");
    layout = new QVBoxLayout(this);

    layout->addWidget(slotComboBox);
    layout->addWidget(cameraComboBox);
    layout->addWidget(okButton);

    connect(okButton, SIGNAL(clicked()), this, SLOT(onOkButtonClicked()));

    slotComboBox->addItem("Slot1");
    slotComboBox->addItem("Slot2");
    GetComboBoxMapForCameras();
}

CameraComboBoxDialog::~CameraComboBoxDialog()
{
    delete cameraComboBox;
    delete okButton;
    delete layout;
}

QString CameraComboBoxDialog::GetSlotComboBoxText()
{
    return slotComboBox->currentText();
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

DatabaseEntity::Camera CameraComboBoxDialog::GetChosenCamera()
{
    return map[cameraComboBox->currentText()];
}

void CameraComboBoxDialog::onOkButtonClicked() {
    accept();
}