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
        explicit ProtectedDataImage(const std::vector<char> &buffer, ImageFormat format = ImageFormat::ImageFormat_Unknown) : IData(DataType::DataType_ProtectedData_Image), buffer_(buffer), format_(format) {}
        explicit ProtectedDataImage(std::vector<char> &&buffer, ImageFormat format = ImageFormat::ImageFormat_Unknown) : IData(DataType::DataType_ProtectedData_Image), buffer_(std::move(buffer)), format_(format) {}
        virtual ~ProtectedDataImage() = default;
        const std::vector<char> buffer_;
        ImageFormat format_;
    };
}