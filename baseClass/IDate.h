#pragma once
namespace upload
{
    enum class DateType // 数据类型枚举
    {
        DateType_Unknown = 0,
        DateType_Image
    };
    class IDate // 所有数据的基类，就像void*一样
    {
    public:
        explicit IDate(const DateType type) : type_(type) {}
        virtual ~IDate() = default;
    private:
       const DateType type_ ;
    };
}
