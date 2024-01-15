#include <Model/Database/DataAccess/ParkingSpace.h>

void DatabaseDataAccess::ParkingSpace::Add(const DatabaseEntity::ParkingSpace& parkingSpace)
{
    QSqlQuery query;
    query.prepare("INSERT INTO parking_space (camera_id, x1, x2, y1, y2, name) VALUES (:camera_id, :x1, :x2, :y1, :y2, :name)");
    query.bindValue(":camera_id", parkingSpace.GetCamera().GetID());
    query.bindValue(":x1", parkingSpace.GetX1());
    query.bindValue(":x2", parkingSpace.GetX2());
    query.bindValue(":y1", parkingSpace.GetY1());
    query.bindValue(":y2", parkingSpace.GetY2());
    query.bindValue(":name", QString::fromStdString(parkingSpace.GetName()));
    if (query.exec())
        qDebug() << "ParkingSpace entry added successfully";
    else throw std::runtime_error(query.lastError().text().toStdString());
}

void DatabaseDataAccess::ParkingSpace::Remove(const DatabaseEntity::ParkingSpace& parkingSpace)
{
    QSqlQuery query;
    query.prepare("DELETE FROM parking_space WHERE parking_space.id = :id");
    query.bindValue(":id", parkingSpace.GetID());

    if (query.exec()) {
        std::cout << "ParkingSpace with ID " << parkingSpace.GetID() << " removed successfully";
    }
    else throw std::runtime_error(query.lastError().text().toStdString());
}

void DatabaseDataAccess::ParkingSpace::RemoveAllFromCamera(const DatabaseEntity::Camera& camera)
{
    std::vector<DatabaseEntity::ParkingSpace> allEntries = FindAllByCamera(camera);
    for (const auto& entry : allEntries)
        Remove(entry);
}

void DatabaseDataAccess::ParkingSpace::Update(const DatabaseEntity::ParkingSpace& parkingSpace)
{
    QSqlQuery query;
    query.prepare("UPDATE parking_space SET x1 = :x1, x2 = :x2, y1 = :y1, y2 = :y2, name = :name WHERE id = :id; ");
    query.bindValue(":id", parkingSpace.GetID());
    query.bindValue(":x1", parkingSpace.GetX1());
    query.bindValue(":x2", parkingSpace.GetX2());
    query.bindValue(":y1", parkingSpace.GetY1());
    query.bindValue(":y2", parkingSpace.GetY2());
    query.bindValue(":name", QString::fromStdString(parkingSpace.GetName()));

    if (query.exec())
        std::cout << "ParkingSpace with ID " << parkingSpace.GetID() << " updated successfully";
    else throw std::runtime_error(query.lastError().text().toStdString());
}

std::vector<DatabaseEntity::ParkingSpace> DatabaseDataAccess::ParkingSpace::FindAll()
{
    std::vector<DatabaseEntity::ParkingSpace> entries;
    QSqlQuery query("SELECT * FROM parking_space");
    DatabaseDataAccess::Camera cameraDataAccess;
    if (!query.isActive())
        throw std::runtime_error(query.lastError().text().toStdString());

    while (query.next()) {
        uint32_t id = query.value("id").toInt();
        DatabaseEntity::Camera camera = cameraDataAccess.FindByID(query.value("camera_id").toInt());
        uint32_t x1 = query.value("x1").toInt();
        uint32_t x2 = query.value("x2").toInt();
        uint32_t y1 = query.value("y1").toInt();
        uint32_t y2 = query.value("y2").toInt();
        std::string name = query.value("name").toString().toStdString();
        DatabaseEntity::ParkingSpace parkingSpace(id, camera, x1, x2, y1, y2, name);

        entries.push_back(parkingSpace);
    }
    return entries;
}

std::vector<DatabaseEntity::ParkingSpace> DatabaseDataAccess::ParkingSpace::FindAllByCamera(const DatabaseEntity::Camera& camera)
{
    std::vector<DatabaseEntity::ParkingSpace> entries;
    QSqlQuery query;
    query.prepare("SELECT * FROM parking_space WHERE parking_space.camera_id = :camera_id");
    query.bindValue(":camera_id", camera.GetID());
    query.exec();

    if (!query.isActive())
        throw std::runtime_error(query.lastError().text().toStdString());

    while (query.next()) {
        uint32_t id = query.value("id").toInt();
        uint32_t x1 = query.value("x1").toInt();
        uint32_t x2 = query.value("x2").toInt();
        uint32_t y1 = query.value("y1").toInt();
        uint32_t y2 = query.value("y2").toInt();
        std::string name = query.value("name").toString().toStdString();
        DatabaseEntity::ParkingSpace parkingSpace(id, camera, x1, x2, y1, y2, name);

        entries.push_back(parkingSpace);
    }
    return entries;
}

DatabaseEntity::ParkingSpace DatabaseDataAccess::ParkingSpace::FindByID(const uint32_t id)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM parking_space WHERE id = :id");
    query.bindValue(":id", id);
    query.exec();
    DatabaseDataAccess::Camera cameraDataAccess;
    if (!query.isActive())
        throw std::runtime_error(query.lastError().text().toStdString());

    DatabaseEntity::ParkingSpace parkingSpace;

    while (query.next()) {
        uint32_t id = query.value("id").toInt();
        DatabaseEntity::Camera camera = cameraDataAccess.FindByID(query.value("camera_id").toInt());
        uint32_t x1 = query.value("x1").toInt();
        uint32_t x2 = query.value("x2").toInt();
        uint32_t y1 = query.value("y1").toInt();
        uint32_t y2 = query.value("y2").toInt();
        std::string name = query.value("name").toString().toStdString();
        parkingSpace = DatabaseEntity::ParkingSpace(id, camera, x1, x2, y1, y2, name);

    }
    return parkingSpace;
}
