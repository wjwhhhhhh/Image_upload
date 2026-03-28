#pragma once
#include "../DataTypes/DataType.h"
#include <string>
namespace upload
{
    ImageFormat getImageFormatFromFilePath(const std::string &filepath);
    std::string getImageStringFromImageFormat(ImageFormat format);
}