#pragma once
namespace upload
{
    enum class DataType // 数据类型枚举
    {
        DataType_Unknown = 0,
        //动力源提供的数据
        DataType_DataFlow_ImageFile=1,

        DataType_DataRead_Image=100,

        DataType_OriginalData_Image=200,
    };
} // namespace upload