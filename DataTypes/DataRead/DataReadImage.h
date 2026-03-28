#pragma once
#include "../DataType.h"
#include "../../baseClass/IData.h"
#include <string>
namespace upload
{
    struct DataReadImage : public IData
    {
    public:
        explicit DataReadImage(const std::string &filepath, ImageFormat format = ImageFormat::ImageFormat_Unknown) : IData(DataType::DataType_DataRead_Image), filepath_(filepath), format_(format) {}
        virtual ~DataReadImage() = default;
        const std::string filepath_;
        ImageFormat format_;
    };
}