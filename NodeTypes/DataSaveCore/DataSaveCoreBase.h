#pragma once
#include "../../baseClass/INode.h"
#include <map>
namespace upload
{
    class DataSaveCoreBase : public INode
    {
    public:
        DataSaveCoreBase(NodeType type = NodeType::NodeType_Input) : INode(type) {}
        virtual ~DataSaveCoreBase() = default;
        InputHandler getInputHandler(std::shared_ptr<IData> data) const override
        {
            std::cout << "[DataSaveCoreBase::onInputHandler]" << std::endl;
            return DataSaveCoreBase::inputHandlers_[data->getType()];
        }
        OutputHandler getOutputHandler(std::shared_ptr<IData> data) const override
        {
            return DataSaveCoreBase::outputHandlers_[data->getType()];
        }

    protected:
        static std::map<DataType, InputHandler> inputHandlers_;
        static std::map<DataType, OutputHandler> outputHandlers_;
        static bool is_loaded_input_;
        static bool is_loaded_output_;
    };
}