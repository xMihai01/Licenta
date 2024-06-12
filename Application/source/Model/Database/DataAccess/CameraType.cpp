#include "Model/Database/DataAccess/CameraType.h"

DatabaseDataAccess::CameraType::CameraType(const QString& usedDatabase)
    : m_usedDatabase(usedDatabase)
{
}

void DatabaseDataAccess::CameraType::Remove(const DatabaseEntity::CameraType& cameraType) 
{
    QSqlQuery query(QSqlDatabase::database(m_usedDatabase));
    query.prepare("DELETE FROM camera_type WHERE camera_type.id = :id");
    query.bindValue(":id", cameraType.GetID());

    if (query.exec())
        std::cout << "CameraType with ID " << cameraType.GetID() << " removed successfully";
    else throw std::runtime_error(query.lastError().text().toStdString());
}

void DatabaseDataAccess::CameraType::RemoveAll()
{
    auto entries = FindAll();
    for (const auto& entry : entries)
        Remove(entry);
}

void DatabaseDataAccess::CameraType::Add(const DatabaseEntity::CameraType& cameraType, const bool ignoreId)
{
    QSqlQuery query(QSqlDatabase::database(m_usedDatabase));
    ignoreId ? query.prepare("INSERT INTO camera_type (type) VALUES (:type)") : query.prepare("INSERT INTO camera_type (id, type) VALUES (:id, :type)");
    if (!ignoreId)
        query.bindValue(":id", cameraType.GetID());
    query.bindValue(":type", DatabaseEntity::CameraType::ConvertTypeToQString(cameraType.GetType()));

    if (query.exec())
        qDebug() << "Entry added successfully";
    else throw std::runtime_error(query.lastError().text().toStdString());
}

std::vector<DatabaseEntity::CameraType> DatabaseDataAccess::CameraType::FindAll()
{
    std::vector<DatabaseEntity::CameraType> entries;
    QSqlQuery query("SELECT * FROM camera_type", QSqlDatabase::database(m_usedDatabase));

    if (!query.isActive())
        throw std::runtime_error(query.lastError().text().toStdString());

    while (query.next()) {
        uint32_t id = query.value("id").toInt();
        DatabaseEntity::CameraType::Type type = DatabaseEntity::CameraType::ConvertQStringToType(query.value("type").toString());

        entries.push_back(DatabaseEntity::CameraType(id, type));
    }
    return entries;
}

DatabaseEntity::CameraType DatabaseDataAccess::CameraType::FindByID(const uint32_t id)
{
    QSqlQuery query(QSqlDatabase::database(m_usedDatabase));
    query.prepare("SELECT * FROM camera_type WHERE camera_type.id = :id");
    query.bindValue(":id", id);
    query.exec();
    if (!query.isActive()) 
        throw std::runtime_error(query.lastError().text().toStdString());

    DatabaseEntity::CameraType cameraTypeEntity;

    while (query.next()) {
        uint32_t id = query.value("id").toInt();
        DatabaseEntity::CameraType::Type type = DatabaseEntity::CameraType::ConvertQStringToType(query.value("type").toString());
        
        cameraTypeEntity = DatabaseEntity::CameraType(id, type);
    }
    return cameraTypeEntity;
}

DatabaseEntity::CameraType DatabaseDataAccess::CameraType::FindByType(const DatabaseEntity::CameraType::Type type)
{
    return FindByID(static_cast<int>(type));
}