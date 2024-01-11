#include <Model/Database/DataAccess/Camera.h>

DatabaseDataAccess::Camera::Camera() {

}

void DatabaseDataAccess::Camera::Add(const DatabaseEntity::Camera& camera)
{
    QSqlQuery query;
    query.prepare("INSERT INTO camera (camera_type_id, location, name) VALUES (:camera_type_id, :location, :name)");
    query.bindValue(":camera_type_id", camera.GetCameraType().GetID());
    query.bindValue(":location", QString::fromStdString(camera.GetLocation()));
    query.bindValue(":name", QString::fromStdString(camera.GetName()));
    if (query.exec())
        qDebug() << "Entry added successfully";
    else throw std::runtime_error(query.lastError().text().toStdString());
}

void DatabaseDataAccess::Camera::Remove(const DatabaseEntity::Camera& camera)
{
    QSqlQuery query;
    query.prepare("DELETE FROM camera WHERE camera.id = :id");
    query.bindValue(":id", camera.GetID());

    if (m_cameraKeyDataAccess.FindByID(camera.GetID()).GetID() != 0)
        m_cameraKeyDataAccess.Remove(m_cameraKeyDataAccess.FindByID(camera.GetID()));

    if (query.exec()) {
        std::cout << "Camera with ID " << camera.GetID() << " removed successfully";
    }
    else throw std::runtime_error(query.lastError().text().toStdString());
}

void DatabaseDataAccess::Camera::Update(const DatabaseEntity::Camera& camera)
{
    QSqlQuery query;
    query.prepare("UPDATE camera SET camera_type_id = :camera_type_id , location = :location , name = :name WHERE id = :id; ");
    query.bindValue(":id", camera.GetID());
    query.bindValue(":camera_type_id", camera.GetCameraType().GetID());
    query.bindValue(":location", QString::fromStdString(camera.GetLocation()));
    query.bindValue(":name", QString::fromStdString(camera.GetName()));

    if (query.exec())
        std::cout << "Camera with ID " << camera.GetID() << " updated successfully";
    else throw std::runtime_error(query.lastError().text().toStdString());
    
}

std::vector<DatabaseEntity::Camera> DatabaseDataAccess::Camera::FindAll()
{
    std::vector<DatabaseEntity::Camera> entries;
    QSqlQuery query("SELECT * FROM camera");
    DatabaseDataAccess::CameraType cameraTypeDataAccess;
    if (!query.isActive())
        throw std::runtime_error(query.lastError().text().toStdString());

    while (query.next()) {
        uint32_t id = query.value("id").toInt();
        DatabaseEntity::CameraType cameraType = cameraTypeDataAccess.FindByID(query.value("camera_type_id").toInt());
        std::string location = query.value("location").toString().toStdString();
        std::string name = query.value("name").toString().toStdString();
        DatabaseEntity::Camera camera(id, cameraType, location, name);
        camera.SetIsLocationAnIndex(m_businessLogic.IsLocationAnIndex(camera));

        entries.push_back(camera);
    }
    return entries;
}

DatabaseEntity::Camera DatabaseDataAccess::Camera::FindByID(const uint32_t id)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM camera WHERE id = :id");
    query.bindValue(":id", id);
    query.exec();
    DatabaseDataAccess::CameraType cameraTypeDataAccess;
    if (!query.isActive())
        throw std::runtime_error(query.lastError().text().toStdString());

    DatabaseEntity::Camera camera;

    while (query.next()) {
        uint32_t id = query.value("id").toInt();
        DatabaseEntity::CameraType cameraType = cameraTypeDataAccess.FindByID(query.value("camera_type_id").toInt());
        std::string location = query.value("location").toString().toStdString();
        std::string name = query.value("name").toString().toStdString();
        camera = DatabaseEntity::Camera(id, cameraType, location, name);
        camera.SetIsLocationAnIndex(m_businessLogic.IsLocationAnIndex(camera));

    }
    return camera;
}
