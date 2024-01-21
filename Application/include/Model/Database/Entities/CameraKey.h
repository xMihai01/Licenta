#pragma once

#include <stdint.h>

#include <Model/Utils/QtKeysEnum.h>

namespace DatabaseEntity {

	class CameraKey {
	public:

		CameraKey(const uint32_t id, const QtKeyEnum key);
		CameraKey(const QtKeyEnum key);
		CameraKey();

		void SetKey(const QtKeyEnum key);
		
		QtKeyEnum GetKey() const;
		uint32_t GetID() const;

	private:

		uint32_t m_id;
		QtKeyEnum m_key;

	};

}