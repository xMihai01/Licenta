#include "Model/Database/BusinessLogic/CameraKey.h"

QtKeyEnum DatabaseBusinessLogic::CameraKey::ConvertIntToKeyEnum(const int key)
{
    return QtKeyEnum(key);
}

int DatabaseBusinessLogic::CameraKey::ConvertKeyEnumToInt(const QtKeyEnum key)
{
    return static_cast<int>(key);
}
