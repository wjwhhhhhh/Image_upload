#include "./DataSaveCoreBase.h"

namespace upload
{
    // 静态成员定义
    std::map<DataType, InputHandler> DataSaveCoreBase::inputHandlers_;
    std::map<DataType, OutputHandler> DataSaveCoreBase::outputHandlers_;
    bool DataSaveCoreBase::is_loaded_input_ = false;
    bool DataSaveCoreBase::is_loaded_output_ = false;
}
