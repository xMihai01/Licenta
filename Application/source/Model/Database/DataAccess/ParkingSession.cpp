#include "Model/Database/DataAccess/ParkingSession.h"

void DatabaseDataAccess::ParkingSession::Add(const DatabaseEntity::ParkingSession& parkingSession)
{
    QSqlQuery query;
    query.prepare("INSERT INTO parking_session (session_id, parking_camera_id, parking_space_id, start_time, end_time) VALUES (:session_id, :parking_camera_id, :parking_space_id, :start_time, :end_time)");
    query.bindValue(":session_id", parkingSession.GetSessionID());
    query.bindValue(":parking_camera_id", parkingSession.GetParkingCameraID());
    query.bindValue(":parking_space_id", parkingSession.GetParkingSpaceID());
    query.bindValue(":start_time", parkingSession.GetStartTime());
    query.bindValue(":end_time", parkingSession.GetEndTime());
    if (query.exec())
        qDebug() << "ParkingSession entry added successfully";
    else throw std::runtime_error(query.lastError().text().toStdString());
}

void DatabaseDataAccess::ParkingSession::Update(const DatabaseEntity::ParkingSession& parkingSession)
{
    QSqlQuery query;
    query.prepare("UPDATE parking_session SET end_time = :end_time WHERE id = :id; ");
    query.bindValue(":id", parkingSession.GetID());
    query.bindValue(":end_time", parkingSession.GetEndTime());

    if (query.exec())
        qDebug() << "ParkingSession with ID " << parkingSession.GetID() << " updated successfully (Set end time)";
    else throw std::runtime_error(query.lastError().text().toStdString());
}

std::vector<DatabaseEntity::ParkingSession> DatabaseDataAccess::ParkingSession::FindAll()
{
    std::vector<DatabaseEntity::ParkingSession> entries;
    QSqlQuery query("SELECT * FROM parking_session");

    if (!query.isActive())
        throw std::runtime_error(query.lastError().text().toStdString());

    while (query.next()) {
        uint32_t id = query.value("id").toInt();
        uint32_t sessionID = query.value("session_id").toInt();
        uint32_t cameraID = query.value("parking_camera_id").toInt();
        uint32_t spaceID = query.value("parking_space_id").toInt();
        QDateTime startTime = query.value("start_time").toDateTime();
        QDateTime endTime = query.value("end_time").toDateTime();

        entries.push_back(DatabaseEntity::ParkingSession(id, sessionID, cameraID, spaceID, startTime, endTime));
    }
    return entries;
}

DatabaseEntity::ParkingSession DatabaseDataAccess::ParkingSession::FindByID(const uint32_t id)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM parking_session WHERE parking_session.id = :id");
    query.bindValue(":id", id);
    query.exec();

    if (!query.isActive())
        throw std::runtime_error(query.lastError().text().toStdString());

    DatabaseEntity::ParkingSession session;

    while (query.next()) {
        uint32_t id = query.value("id").toInt();
        uint32_t sessionID = query.value("session_id").toInt();
        uint32_t cameraID = query.value("parking_camera_id").toInt();
        uint32_t spaceID = query.value("parking_space_id").toInt();
        QDateTime startTime = query.value("start_time").toDateTime();
        QDateTime endTime = query.value("end_time").toDateTime();

        session = DatabaseEntity::ParkingSession(id, sessionID, cameraID, spaceID, startTime, endTime);
    }
    return session;
}

std::vector<DatabaseEntity::ParkingSession> DatabaseDataAccess::ParkingSession::FindBySessionID(const uint32_t sessionID)
{
    std::vector<DatabaseEntity::ParkingSession> entries;
    QSqlQuery query;
    query.prepare("SELECT * FROM parking_session WHERE session_id = :session_id");
    query.bindValue(":session_id", sessionID);
    query.exec();

    if (!query.isActive())
        throw std::runtime_error(query.lastError().text().toStdString());

    while (query.next()) {
        uint32_t id = query.value("id").toInt();
        uint32_t sessionID = query.value("session_id").toInt();
        uint32_t cameraID = query.value("parking_camera_id").toInt();
        uint32_t spaceID = query.value("parking_space_id").toInt();
        QDateTime startTime = query.value("start_time").toDateTime();
        QDateTime endTime = query.value("end_time").toDateTime();

        entries.push_back(DatabaseEntity::ParkingSession(id, sessionID, cameraID, spaceID, startTime, endTime));
    }
    return entries;
}
