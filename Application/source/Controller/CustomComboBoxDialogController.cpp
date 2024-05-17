#include "Controller/CustomComboBoxDialogController.h"

void CustomComboBoxDialogController::GetComboBoxMapForCameras(QHash<QString, DatabaseEntity::Camera>& map)
{
    std::vector<DatabaseEntity::Camera> cameras = m_database.ToCamera().FindAll();

    for (auto& camera : cameras) {
        int id = camera.GetID();
        QString comboBoxString = QString::fromStdString(camera.GetName()) + " (" + QString::number(id) + ")";
        map[comboBoxString] = camera;
    }
}

void CustomComboBoxDialogController::GetComboBoxMapForExitCameras(QHash<QString, DatabaseEntity::Camera>& map)
{
    std::vector<DatabaseEntity::Camera> cameras = m_database.ToCamera().FindAll();

    for (auto& camera : cameras) {
        if (camera.GetCameraType().GetType() == DatabaseEntity::CameraType::Type::EXIT) {
            int id = camera.GetID();
            QString comboBoxString = QString::fromStdString(camera.GetName()) + " (" + QString::number(id) + ")";
            map[comboBoxString] = camera;
        }
    }
}

void CustomComboBoxDialogController::GetComboBoxMapForOngoingSessions(QHash<QString, DatabaseEntity::Session>& map)
{
    std::vector<DatabaseEntity::Session> sessions = m_database.ToSession().FindAllOngoingSessions();

    for (const auto& session : sessions) {
        int id = session.GetID();
        QString licensePlateText = QString::fromStdString(session.GetLicensePlate());
        QString comboBoxString = QString::number(id) + "(" + licensePlateText + ")";
        map[comboBoxString] = session;
    }
}

void CustomComboBoxDialogController::GetComboBoxMapForCameraType(QHash<QString, DatabaseEntity::CameraType>& map)
{
    std::vector<DatabaseEntity::CameraType> cameraTypes = m_database.ToCameraType().FindAll();

    for (const auto& cameraType : cameraTypes) {
        DatabaseBusinessLogic::CameraType cameraTypeBL;
        QString typeText = DatabaseEntity::CameraType::ConvertTypeToQString(cameraType.GetType());
        map[typeText] = cameraType;
    }
}
