#include "./DataProtectCoreBase.h"

namespace upload
{
    // 静态成员定义
    std::map<DataType, InputHandler> DataProtectCoreBase::inputHandlers_;
    std::map<DataType, OutputHandler> DataProtectCoreBase::outputHandlers_;
    bool DataProtectCoreBase::is_loaded_input_ = false;
    bool DataProtectCoreBase::is_loaded_output_ = false;
}
