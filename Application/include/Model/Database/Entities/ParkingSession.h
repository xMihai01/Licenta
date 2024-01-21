#pragma once

#include <QDateTime>

namespace DatabaseEntity {

	class ParkingSession {
	public:

		ParkingSession();
		ParkingSession(const uint32_t id, const uint32_t sessionID, const uint32_t parkingCameraID, const uint32_t parkingSpaceID, const QDateTime& startTime, const QDateTime& endTime);
		ParkingSession(const uint32_t sessionID, const uint32_t parkingCameraID, const uint32_t parkingSpaceID, const QDateTime& startTime, const QDateTime& endTime);

		uint32_t GetID() const;
		uint32_t GetSessionID() const;
		uint32_t GetParkingCameraID() const;
		uint32_t GetParkingSpaceID() const;
		QDateTime GetStartTime() const;
		QDateTime GetEndTime() const;

		void SetSessionID(const uint32_t sessionID);
		void SetParkingCameraID(const uint32_t cameraID);
		void SetParkingSpaceID(const uint32_t spaceID);
		void SetStartTime(const QDateTime& startTime);
		void SetEndTime(const QDateTime& endTime);

	private:

		uint32_t m_id;
		uint32_t m_sessionID;
		uint32_t m_parkingCameraID;
		uint32_t m_parkingSpaceID;
		QDateTime m_startTime;
		QDateTime m_endTime;

	};
	
}