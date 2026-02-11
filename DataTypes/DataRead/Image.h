#pragma once
#include "../DataType.h"
#include "../../baseClass/IData.h"
#include <string>
namespace upload
{
    struct IDataReadImage : public IData
    {
    public:
        explicit IDataReadImage(const std::string& filepath) : IData(DataType::DataType_DataRead_Image), filepath_(filepath) {}
        virtual ~IDataReadImage() = default;
        const std::string filepath_;
    };
}