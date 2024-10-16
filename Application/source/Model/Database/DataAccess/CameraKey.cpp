#include <Model/Database/DataAccess/CameraKey.h>

DatabaseDataAccess::CameraKey::CameraKey(const QString& usedDatabase) 
    : m_usedDatabase(usedDatabase)
{
}

void DatabaseDataAccess::CameraKey::Add(const DatabaseEntity::CameraKey& cameraKey)
{
    QSqlQuery query(QSqlDatabase::database(m_usedDatabase));
    query.prepare("INSERT INTO camera_key (camera_id, key) VALUES (:id, :key)");
    query.bindValue(":id", cameraKey.GetID());
    query.bindValue(":key", DatabaseEntity::CameraKey::ConvertKeyEnumToInt(cameraKey.GetKey()));
    if (query.exec())
        qDebug() << "CameraKey entry added successfully";
    else throw std::runtime_error(query.lastError().text().toStdString());
}

void DatabaseDataAccess::CameraKey::Remove(const DatabaseEntity::CameraKey& cameraKey)
{
    QSqlQuery query(QSqlDatabase::database(m_usedDatabase));
    query.prepare("DELETE FROM camera_key WHERE camera_id = :id");
    query.bindValue(":id", cameraKey.GetID());

    if (query.exec())
        std::cout << "CameraKey with ID " << cameraKey.GetID() << " removed successfully";
    else throw std::runtime_error(query.lastError().text().toStdString());
}

void DatabaseDataAccess::CameraKey::Update(const DatabaseEntity::CameraKey& cameraKey)
{
    QSqlQuery query(QSqlDatabase::database(m_usedDatabase));
    query.prepare("UPDATE camera_key SET key = :key WHERE camera_id = :id; ");
    query.bindValue(":id", cameraKey.GetID());
    query.bindValue(":key", DatabaseEntity::CameraKey::ConvertKeyEnumToInt(cameraKey.GetKey()));

    if (query.exec())
        std::cout << "CameraKey with ID " << cameraKey.GetID() << " updated successfully";
    else throw std::runtime_error(query.lastError().text().toStdString());

}

DatabaseEntity::CameraKey DatabaseDataAccess::CameraKey::FindByID(const uint32_t id)
{
    QSqlQuery query(QSqlDatabase::database(m_usedDatabase));
    query.prepare("SELECT * FROM camera_key WHERE camera_id = :id");
    query.bindValue(":id", id);
    query.exec();

    if (!query.isActive())
        throw std::runtime_error(query.lastError().text().toStdString());

    DatabaseEntity::CameraKey camera;

    while (query.next()) {
        uint32_t id = query.value("camera_id").toInt();
        QtKeyEnum key = DatabaseEntity::CameraKey::ConvertIntToKeyEnum(query.value("key").toInt());

        camera = DatabaseEntity::CameraKey(id, key);
    }
    return camera;
}

DatabaseEntity::CameraKey DatabaseDataAccess::CameraKey::FindByKey(const QtKeyEnum key)
{
    QSqlQuery query(QSqlDatabase::database(m_usedDatabase));
    query.prepare("SELECT * FROM camera_key WHERE key = :key");
    query.bindValue(":key", static_cast<int>(key));
    query.exec();

    if (!query.isActive())
        throw std::runtime_error(query.lastError().text().toStdString());

    DatabaseEntity::CameraKey camera;

    while (query.next()) {
        uint32_t id = query.value("camera_id").toInt();
        QtKeyEnum key = DatabaseEntity::CameraKey::ConvertIntToKeyEnum(query.value("key").toInt());

        camera = DatabaseEntity::CameraKey(id, key);
    }
    return camera;
}

std::vector<DatabaseEntity::CameraKey> DatabaseDataAccess::CameraKey::FindAll()
{
    std::vector<DatabaseEntity::CameraKey> entries;
    QSqlQuery query("SELECT * FROM camera_key", QSqlDatabase::database(m_usedDatabase));

    if (!query.isActive())
        throw std::runtime_error(query.lastError().text().toStdString());

    while (query.next()) {
        uint32_t id = query.value("camera_id").toInt();
        QtKeyEnum key = DatabaseEntity::CameraKey::ConvertIntToKeyEnum(query.value("key").toInt());

        entries.push_back(DatabaseEntity::CameraKey(id, key));
    }
    return entries;
}
