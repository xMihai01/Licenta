#pragma once

#include <stdint.h>

namespace DatabaseEntity {

	class CameraType {
		
	public:

		enum class Type {
			ENTRANCE,
			EXIT,
			PARKING,
			INVALID
		};

	public:

		CameraType(const uint32_t id, const Type type);
		CameraType(const Type type);
		CameraType();

		void SetType(const Type type);
		
		Type GetType() const;
		uint32_t GetID() const;

	private:

		uint32_t m_id;
		Type m_type;

	};

}