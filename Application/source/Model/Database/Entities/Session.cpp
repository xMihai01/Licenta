#include "Model/Database/Entities/Session.h"

DatabaseEntity::Session::Session()
{
	m_id = 0;
}

DatabaseEntity::Session::Session(const uint32_t id, const std::string& licensePlate, const QDateTime& entranceTime, const QDateTime& exitTime, const std::string& secretID)
	: m_id(id)
	, m_licensePlate(licensePlate)
	, m_entranceTime(entranceTime)
	, m_exitTime(exitTime)
	, m_secretID(secretID)
{
}

DatabaseEntity::Session::Session(const std::string& licensePlate, const QDateTime& entranceTime, const QDateTime& exitTime, const std::string& secretID)
	: m_licensePlate(licensePlate)
	, m_entranceTime(entranceTime)
	, m_exitTime(exitTime)
	, m_secretID(secretID)
{
	m_id = 0;
}

DatabaseEntity::Session::Session(const std::string& licensePlate, const QDateTime& entranceTime, const QDateTime& exitTime)
	: m_licensePlate(licensePlate)
	, m_entranceTime(entranceTime)
	, m_exitTime(exitTime)
{
}

DatabaseEntity::Session::Session(const std::string& licensePlate, const QDateTime& entranceTime)
	: m_licensePlate(licensePlate)
	, m_entranceTime(entranceTime)
{
}

void DatabaseEntity::Session::SetLicensePlate(const std::string& licensePlate)
{
	m_licensePlate = licensePlate;
}

void DatabaseEntity::Session::SetEntranceTime(const QDateTime& entranceTime)
{
	m_entranceTime = entranceTime;
}

void DatabaseEntity::Session::SetExitTime(const QDateTime& exitTime)
{
	m_exitTime = exitTime;
}

void DatabaseEntity::Session::SetSecretID(const std::string& secretID)
{
	m_secretID = secretID;
}

uint32_t DatabaseEntity::Session::GetID() const
{
	return m_id;
}

std::string DatabaseEntity::Session::GetLicensePlate() const
{
	return m_licensePlate;
}

QDateTime DatabaseEntity::Session::GetEntranceTime() const
{
	return m_entranceTime;
}

QDateTime DatabaseEntity::Session::GetExitTime() const
{
	return m_exitTime;
}

std::string DatabaseEntity::Session::GetSecretID() const
{
	return m_secretID;
}
