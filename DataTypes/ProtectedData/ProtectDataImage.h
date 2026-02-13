#pragma once 
#include "../DataType.h"
#include "../../baseClass/IData.h"
#include <string>
#include <vector>
namespace upload
{
    struct ProtectedDataImage : public IData
    {
    public:
        explicit ProtectedDataImage() : IData(DataType::DataType_ProtectedData_Image) {}
        explicit ProtectedDataImage(const std::vector<char>& buffer) : IData(DataType::DataType_ProtectedData_Image), buffer_(buffer) {}
        explicit ProtectedDataImage(std::vector<char>&& buffer) : IData(DataType::DataType_ProtectedData_Image), buffer_(std::move(buffer)) {}
        virtual ~ProtectedDataImage() = default;
        const std::vector<char> buffer_;
    };
}