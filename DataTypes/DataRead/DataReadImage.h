#pragma once
#include "../DataType.h"
#include "../../baseClass/IData.h"
#include <string>
namespace upload
{
    struct DataReadImage : public IData
    {
    public:
        explicit DataReadImage(const std::string& filepath) : IData(DataType::DataType_DataRead_Image), filepath_(filepath) {}
        virtual ~DataReadImage() = default;
        const std::string filepath_;
    };
}