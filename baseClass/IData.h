#pragma once
#include "../DataTypes/DataType.h"
namespace upload
{
    struct IData // 所有数据的基类，就像void*一样
    {
    public:
        explicit IData(const DataType type) : type_(type) {}
        virtual ~IData() = default;
       const DataType type_ ;
        DataType getType() const noexcept { return type_; }
    };
}
