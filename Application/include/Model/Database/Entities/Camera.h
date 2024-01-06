#pragma once

#include <Model/Database/Entities/CameraType.h>

#include <string>

namespace DatabaseEntity {

	class Camera {

	public:

		Camera();
		Camera(const uint32_t id, const DatabaseEntity::CameraType& cameraType, const std::string& location, const std::string& name);
		Camera(const DatabaseEntity::CameraType& cameraType, const std::string& location, const std::string& name);

		uint32_t GetID() const;
		DatabaseEntity::CameraType GetCameraType() const;
		std::string GetLocation() const;
		std::string GetName() const;

		void SetCameraType(const DatabaseEntity::CameraType& cameraType);
		void SetLocation(const std::string& location);
		void SetName(const std::string& name);

		bool IsLocationAnIndex() const;
		void SetIsLocationAnIndex(bool isindex);

	private:

		uint32_t m_id;
		DatabaseEntity::CameraType m_cameraType;
		std::string m_location;
		std::string m_name;

		bool isLocationAnIndex = false; // not actually present in db table
	};
}