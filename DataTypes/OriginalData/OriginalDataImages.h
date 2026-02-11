#pragma once
#include "../DataType.h"
#include "../../baseClass/IData.h"
#include <string>
#include <vector>
namespace upload
{
    struct OriginalDataImage : public IData
    {
    public:
        explicit OriginalDataImage() : IData(DataType::DataType_OriginalData_Image) {}
        explicit OriginalDataImage(const std::vector<char>& buffer) : IData(DataType::DataType_OriginalData_Image), buffer_(buffer) {}
        explicit OriginalDataImage(std::vector<char>&& buffer) : IData(DataType::DataType_OriginalData_Image), buffer_(std::move(buffer)) {}
        virtual ~OriginalDataImage() = default;
        const std::vector<char> buffer_;
    };
}