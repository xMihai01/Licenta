#pragma once

#include <string>
#include <QDateTime>

namespace DatabaseEntity {

	class Session {

	public:

		Session();
		Session(const uint32_t id, const std::string& licensePlate, const QDateTime& entranceTime, const QDateTime& exitTime, const std::string& secretID);
		Session(const std::string& licensePlate, const QDateTime& entranceTime, const QDateTime& exitTime, const std::string& secretID);
		Session(const std::string& licensePlate, const QDateTime& entranceTime, const QDateTime& exitTime);
		Session(const std::string& licensePlate, const QDateTime& entranceTime);
		Session(const std::string& licensePlate);

		void SetLicensePlate(const std::string& licensePlate);
		void SetEntranceTime(const QDateTime& entranceTime);
		void SetExitTime(const QDateTime& exitTime);
		void SetSecretID(const std::string& secretID);

		uint32_t GetID() const;
		std::string GetLicensePlate() const;
		QDateTime GetEntranceTime() const;
		QDateTime GetExitTime() const;
		std::string GetSecretID() const;

	private:

		uint32_t m_id;
		std::string m_licensePlate;
		QDateTime m_entranceTime;
		QDateTime m_exitTime;
		std::string m_secretID;

	};

}