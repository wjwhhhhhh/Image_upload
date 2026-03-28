#pragma once
#include "../../baseClass/INode.h"
#include "../../DataTypes/OriginalData/OriginalDataImages.h"
#include "../../DataTypes/ProtectedData/ProtectDataImage.h"
#include <map>
namespace upload
{
    class DataProtectCoreBase : public INode
    {
    public:
        DataProtectCoreBase(NodeType type = NodeType::NodeType_Bidirectional) : INode(type) {}
        virtual ~DataProtectCoreBase() = default;
        InputHandler getInputHandler(std::shared_ptr<IData> data) const override
        {
            std::cout << "[DataProtectCoreBase::onInputHandler]" << std::endl;
            return DataProtectCoreBase::inputHandlers_[data->getType()];
        }
        OutputHandler getOutputHandler(std::shared_ptr<IData> data) const override
        {
            return DataProtectCoreBase::outputHandlers_[data->getType()];
        }

    protected:
        static std::map<DataType, InputHandler> inputHandlers_;
        static std::map<DataType, OutputHandler> outputHandlers_;
        static bool is_loaded_input_;
        static bool is_loaded_output_;
    };
}