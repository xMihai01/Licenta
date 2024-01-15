#pragma once

#include <Model/Database/Entities/Camera.h>

#include <string>

namespace DatabaseEntity {

	class ParkingSpace {

	public:

		ParkingSpace();
		ParkingSpace(const uint32_t id, const DatabaseEntity::Camera& cameraID, const uint32_t x1, const uint32_t x2, const uint32_t y1, const uint32_t y2, const std::string& name);
		ParkingSpace(const DatabaseEntity::Camera& cameraID, const uint32_t x1, const uint32_t x2, const uint32_t y1, const uint32_t y2, const std::string& name);

		void SetX1(const uint32_t x1);
		void SetX2(const uint32_t x2);
		void SetY1(const uint32_t y1);
		void SetY2(const uint32_t y2);
		
		void SetName(const std::string& name);

		uint32_t GetID() const;
		DatabaseEntity::Camera GetCamera() const;
		uint32_t GetX1() const;
		uint32_t GetX2() const;
		uint32_t GetY1() const;
		uint32_t GetY2() const;
		std::string GetName() const;

	private:

		uint32_t m_id;
		DatabaseEntity::Camera m_camera;
		
		uint32_t m_x1;
		uint32_t m_x2;
		uint32_t m_y1;
		uint32_t m_y2;

		std::string m_name;

	};

}