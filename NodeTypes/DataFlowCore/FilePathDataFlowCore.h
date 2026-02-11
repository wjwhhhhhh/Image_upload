#pragma once
#include "DataFlowCoreBase.h"
#include <iostream>
namespace upload
{
    class FilePathDataFlowCore : public DataFlowCoreBase
    {
    public:
        FilePathDataFlowCore() : DataFlowCoreBase(NodeType::NodeType_Input) {
        }
        virtual ~FilePathDataFlowCore() = default;
        InputHandler getInputHandler(std::shared_ptr<IData> data) const override
        {
            return nullptr;
        }
        OutputHandler getOutputHandler(std::shared_ptr<IData> data) const override
        {
            return FilePathDataFlowCore::outputHandlers_[(data->getType())];
        }
        void load() override
        {
            outputHandlers_[DataType::DataType_DataFlow_ImageFile] = [this](std::shared_ptr<IData> data)
            {
                return onOutputImageFile(data);
            }; //用来设置输出处理函数
        }
    private:
        std::shared_ptr<IData> onOutputImageFile(std::shared_ptr<IData> data);
        static std::map<DataType, InputHandler> inputHandlers_;
        static std::map<DataType, OutputHandler> outputHandlers_;
    };
} // namespace upload
