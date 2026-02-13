#include "./DataProtectAESNode.h"
namespace upload {
    void DataProtectAESNode::loadInput() {
        if(DataProtectAESNode::is_loaded_input_)
        {
            return;
        }
        DataProtectAESNode::inputHandlers_[DataType::DataType_DataRead_Image] = [this](std::shared_ptr<IData> data)
        {
            return onInputImageData(data);
        }; //用来设置输入处理函数

        DataProtectAESNode::is_loaded_input_ = true;
    }

    void DataProtectAESNode::loadOutput() {
        if(DataProtectAESNode::is_loaded_output_)
        {
            return;
        }

        DataProtectAESNode::outputHandlers_[DataType::DataType_ProtectedData_Image] = [this](std::shared_ptr<IData> data)
        {
            return onOutputImageData(data);
        }; //用来设置输出处理函数

        DataProtectAESNode::is_loaded_output_ = true;
    }

    void DataProtectAESNode::onInputImageData(std::shared_ptr<IData> data) {
        data_=data;
    }

    std::shared_ptr<IData> DataProtectAESNode::onOutputImageData(std::shared_ptr<IData> data) {
        // AES加密逻辑实现
        std::cout << "[DataProtectAESNode::onOutputImageData] AES encrypting image data." << std::endl;      
        return data;
    }
} // namespace upload