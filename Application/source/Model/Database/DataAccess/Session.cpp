#include <Model/Database/DataAccess/Session.h>

void DatabaseDataAccess::Session::Add(const DatabaseEntity::Session& session)
{
    QSqlQuery query;
    query.prepare("INSERT INTO session (entrance_time, exit_time, license_plate, secret_id) VALUES (:entrance_time, :exit_time, :license_plate, :secret_id)");
    query.bindValue(":entrance_time", session.GetEntranceTime());
    query.bindValue(":exit_time", session.GetExitTime());
    query.bindValue(":license_plate", QString::fromStdString(session.GetLicensePlate()));
    query.bindValue(":secret_id", QString::fromStdString(session.GetSecretID()));
    if (query.exec())
        qDebug() << "Session entry added successfully";
    else throw std::runtime_error(query.lastError().text().toStdString());
}

void DatabaseDataAccess::Session::Remove(const DatabaseEntity::Session& session)
{
    QSqlQuery query;
    query.prepare("DELETE FROM session WHERE session.id = :id");
    query.bindValue(":id", session.GetID());

    // TODO: DELETE PARKING SESSION 

    if (query.exec()) {
        qDebug() << "Session with ID " << session.GetID() << " removed successfully";
    }
    else throw std::runtime_error(query.lastError().text().toStdString());
}

void DatabaseDataAccess::Session::Update(const DatabaseEntity::Session& session)
{
    QSqlQuery query;
    query.prepare("UPDATE session SET exit_time = :exit_time , license_plate = :license_plate WHERE id = :id; ");
    query.bindValue(":id", session.GetID());
    query.bindValue(":exit_time", session.GetExitTime());
    query.bindValue(":license_plate", QString::fromStdString(session.GetLicensePlate()));

    if (query.exec())
        qDebug() << "Session with ID " << session.GetID() << " updated successfully";
    else throw std::runtime_error(query.lastError().text().toStdString());
}

std::vector<DatabaseEntity::Session> DatabaseDataAccess::Session::FindAll()
{
    std::vector<DatabaseEntity::Session> entries;
    QSqlQuery query("SELECT * FROM session");

    if (!query.isActive())
        throw std::runtime_error(query.lastError().text().toStdString());

    while (query.next()) {
        uint32_t id = query.value("id").toInt();
        QDateTime entranceTime = query.value("entrance_time").toDateTime();
        QDateTime exitTime = query.value("exit_time").toDateTime();
        std::string licensePlate = query.value("license_plate").toString().toStdString();
        std::string secretID = query.value("secret_id").toString().toStdString();

        entries.push_back(DatabaseEntity::Session(id, licensePlate, entranceTime, exitTime, secretID));
    }
    return entries;
}

DatabaseEntity::Session DatabaseDataAccess::Session::FindByID(const uint32_t id)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM session WHERE session.id = :id");
    query.bindValue(":id", id);
    query.exec();

    if (!query.isActive())
        throw std::runtime_error(query.lastError().text().toStdString());

    DatabaseEntity::Session session;

    while (query.next()) {
        uint32_t id = query.value("id").toInt();
        QDateTime entranceTime = query.value("entrance_time").toDateTime();
        QDateTime exitTime = query.value("exit_time").toDateTime();
        std::string licensePlate = query.value("license_plate").toString().toStdString();
        std::string secretID = query.value("secret_id").toString().toStdString();
        session = DatabaseEntity::Session(id, licensePlate, entranceTime, exitTime, secretID);
    }
    return session;
}

std::vector<DatabaseEntity::Session> DatabaseDataAccess::Session::FindByLicensePlate(const std::string& licensePlate)
{
    std::vector<DatabaseEntity::Session> entries;
    QSqlQuery query;
    query.prepare("SELECT * FROM session WHERE license_plate = :license_plate");
    query.bindValue(":license_plate", QString::fromStdString(licensePlate));
    query.exec();

    if (!query.isActive())
        throw std::runtime_error(query.lastError().text().toStdString());

    while (query.next()) {
        uint32_t id = query.value("id").toInt();
        QDateTime entranceTime = query.value("entrance_time").toDateTime();
        QDateTime exitTime = query.value("exit_time").toDateTime();
        std::string licensePlate = query.value("license_plate").toString().toStdString();
        std::string secretID = query.value("secret_id").toString().toStdString();

        entries.push_back(DatabaseEntity::Session(id, licensePlate, entranceTime, exitTime, secretID));
    }
    return entries;
}

DatabaseEntity::Session DatabaseDataAccess::Session::FindBySecretID(const std::string& secretID)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM session WHERE secret_id = :secretID");
    query.bindValue(":secretID", QString::fromStdString(secretID));
    query.exec();

    if (!query.isActive())
        throw std::runtime_error(query.lastError().text().toStdString());

    DatabaseEntity::Session session;

    while (query.next()) {
        uint32_t id = query.value("id").toInt();
        QDateTime entranceTime = query.value("entrance_time").toDateTime();
        QDateTime exitTime = query.value("exit_time").toDateTime();
        std::string licensePlate = query.value("license_plate").toString().toStdString();
        std::string secretID = query.value("secret_id").toString().toStdString();
        session = DatabaseEntity::Session(id, licensePlate, entranceTime, exitTime, secretID);
    }
    return session;
}