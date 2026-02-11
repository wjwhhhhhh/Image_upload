#pragma once
#include "DataFlowCoreBase.h"
#include <iostream>
namespace upload
{
    class FilePathDataFlowCore : public DataFlowCoreBase
    {
    public:
        FilePathDataFlowCore() : DataFlowCoreBase(NodeType::NodeType_Output) {
        }
        virtual ~FilePathDataFlowCore() = default;
        InputHandler getInputHandler(std::shared_ptr<IData> data) const override
        {
            return FilePathDataFlowCore::inputHandlers_[data->getType()];
        }
        OutputHandler getOutputHandler(std::shared_ptr<IData> data) const override
        {
            return FilePathDataFlowCore::outputHandlers_[data->getType()];
        }
        void loadOutput() override
        {
            if(FilePathDataFlowCore::is_loaded_output_)
            {
                return;
            }
            FilePathDataFlowCore::outputHandlers_[DataType::DataType_DataFlow_ImageFile] = [this](std::shared_ptr<IData> data)
            {
                return onOutputImageFile(data);
            }; //用来设置输出处理函数

            FilePathDataFlowCore::is_loaded_output_ = true;
        }

        void loadInput() override {
            if (FilePathDataFlowCore::is_loaded_input_)
                return;
            is_loaded_input_ = true;
        }
    private:
        std::shared_ptr<IData> onOutputImageFile(std::shared_ptr<IData> data);
        static std::map<DataType, InputHandler> inputHandlers_;
        static std::map<DataType, OutputHandler> outputHandlers_;
        static bool is_loaded_input_;
        static bool is_loaded_output_;
    };
} // namespace upload
