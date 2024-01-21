#pragma once

#include <stdint.h>

#include <Model/Database/Entities/CameraKey.h>

namespace DatabaseBusinessLogic {

	class CameraKey {
		
	public:

		QtKeyEnum ConvertIntToKeyEnum(const int key);
		int ConvertKeyEnumToInt(const QtKeyEnum key);

	};

}