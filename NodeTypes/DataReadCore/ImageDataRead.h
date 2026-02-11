#pragma once
#include "DataReadCoreBase.h"
#include <iostream>
#include <memory>
namespace upload
{
    class ImageDataRead : public DataReadCoreBase
    {
    public:
        ImageDataRead() : DataReadCoreBase(NodeType::NodeType_Bidirectional) {
        }
        virtual ~ImageDataRead() = default;
        InputHandler getInputHandler(std::shared_ptr<IData> data) const override
        {
            std::cout << "[ImageDataRead::onInputHandler]" << std::endl;
            return ImageDataRead::inputHandlers_[data->getType()];
        }
        OutputHandler getOutputHandler(std::shared_ptr<IData> data) const override
        {
            return ImageDataRead::outputHandlers_[data->getType()];
        }
        void loadOutput() override
        {
            if(ImageDataRead::is_loaded_output_)
            {
                return;
            }

            ImageDataRead::outputHandlers_[DataType::DataType_OriginalData_Image] = [this](std::shared_ptr<IData> data)
            {
                return onOutputImageFile(data);
            }; //用来设置输出处理函数

            ImageDataRead::is_loaded_output_ = true;
        }

        void loadInput() override {
            if(ImageDataRead::is_loaded_input_)
            {
                return;
            }
            ImageDataRead::inputHandlers_[DataType::DataType_DataRead_Image] = [this](std::shared_ptr<IData> data)
            {
                return onInputImageFile(data);
            }; //用来设置输入处理函数

            ImageDataRead::is_loaded_input_ = true;
        }
    private:
        void onInputImageFile(std::shared_ptr<IData> data);
        std::shared_ptr<IData> onOutputImageFile(std::shared_ptr<IData> data);
        static std::map<DataType, InputHandler> inputHandlers_;
        static std::map<DataType, OutputHandler> outputHandlers_;
        static bool is_loaded_input_;
        static bool is_loaded_output_;
    };
} // namespace upload
