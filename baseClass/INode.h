#pragma once
#include "IData.h"
#include <memory>
#include <stdexcept>
#include <map>
#include <string>
#include <iostream>
namespace upload
{

    enum class NodeType
    {
        NodeType_Input,
        NodeType_Output,
        NodeType_Bidirectional
    };

    class INodeInterface
    {
    public:
        virtual ~INodeInterface() = default;
        virtual void Input(std::shared_ptr<IData> data) = 0;
        virtual std::shared_ptr<IData> Output(std::shared_ptr<IData> data) = 0;
    };
    using InputHandler = std::function<void(std::shared_ptr<IData>)>;
    using OutputHandler = std::function<std::shared_ptr<IData>(std::shared_ptr<IData>)>;

    class INode : public INodeInterface
    {
    public:
        explicit INode(NodeType type) : type_(type) {}
        virtual ~INode() = default;

        // 公共入口，阻止子类直接重写，先做检查再委托实现
        void Input(std::shared_ptr<IData> data) final
        {
            if (!supportsInput())
            {
                throw std::logic_error("Node does not support Input");
            }
            auto handler = getInputHandler(data);
            if (!handler)
            {
                std::cerr << "No input handler for data type: " << static_cast<int>(data->getType()) << std::endl;
            }
            getInputHandler(data)(data);
        }
        std::shared_ptr<IData> Output(std::shared_ptr<IData> data) final
        {
            if (!supportsOutput())
            {
                throw std::logic_error("Node does not support Output");
            }
            auto handler = getOutputHandler(data);
            if (!handler)
            {
                std::cerr << "No output handler for data type: " << static_cast<int>(data->getType()) << std::endl;
            }
            return getOutputHandler(data)(data);
        }

        void setConfig(const std::string& config)
        {
            config_ = config;
        }
        NodeType getType() const noexcept { return type_; }

    protected:

        virtual InputHandler getInputHandler(std::shared_ptr<IData> data) const=0;
        virtual OutputHandler getOutputHandler(std::shared_ptr<IData> data) const=0;
        virtual void load()=0;
        bool supportsInput() const noexcept
        {
            return type_ == NodeType::NodeType_Input || type_ == NodeType::NodeType_Bidirectional;
        }
        bool supportsOutput() const noexcept
        {
            return type_ == NodeType::NodeType_Output || type_ == NodeType::NodeType_Bidirectional;
        }

    protected:
        const NodeType type_;
        std::shared_ptr<INode> InputData_;
        std::shared_ptr<INode> OutputData_;
        std::string config_;
    };

}