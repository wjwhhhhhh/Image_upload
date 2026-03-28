#pragma once
namespace upload
{
    enum class DataType // 数据类型枚举
    {
        DataType_Unknown = 0,
        // 动力源提供的数据
        DataType_DataFlow_ImageFile = 1,

        DataType_DataRead_Image = 100,

        DataType_OriginalData_Image = 200,

        DataType_ProtectedData_Image = 300,
    };

    enum class ImageFormat
    {
        ImageFormat_Unknown = 0,
        ImageFormat_JPEG,
        ImageFormat_PNG,
        ImageFormat_BMP,
        ImageFormat_GIF,
        ImageFormat_TIFF
    };
} // namespace upload