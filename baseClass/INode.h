#pragma once
#include "IDate.h"
#include <memory>
#include <stdexcept>
#include <map>
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
        virtual void Input(std::shared_ptr<IDate> data) = 0;
        virtual std::shared_ptr<IDate> Output(std::shared_ptr<IDate> data) = 0;
    };
    using InputHandler = std::function<void(std::shared_ptr<IDate>)>;
    using OutputHandler = std::function<std::shared_ptr<IDate>(std::shared_ptr<IDate>)>;
    class INode : public INodeInterface
    {
    public:
        explicit INode(NodeType type) : type_(type) {}
        virtual ~INode() = default;

        // 公共入口，阻止子类直接重写，先做检查再委托实现
        void Input(std::shared_ptr<IDate> data) final
        {
            if (!supportsInput())
            {
                throw std::logic_error("Node does not support Input");
            }
            getInputHandler(data)(data);
        }
        std::shared_ptr<IDate> Output(std::shared_ptr<IDate> data) final
        {
            if (!supportsOutput())
            {
                throw std::logic_error("Node does not support Output");
            }
            return getOutputHandler(data)(data);
        }

        NodeType getType() const noexcept { return type_; }

    protected:
        // 子类实现具体行为
        void onInput(std::shared_ptr<IDate> data)
        {
            // 默认实现：不处理输入数据
        }
        std::shared_ptr<IDate> onOutput(std::shared_ptr<IDate> data)
        {
            return nullptr;
        }

        virtual InputHandler getInputHandler(std::shared_ptr<IDate> data) const=0;
        virtual OutputHandler getOutputHandler(std::shared_ptr<IDate> data) const=0;
        bool supportsInput() const noexcept
        {
            return type_ == NodeType::NodeType_Input || type_ == NodeType::NodeType_Bidirectional;
        }
        bool supportsOutput() const noexcept
        {
            return type_ == NodeType::NodeType_Output || type_ == NodeType::NodeType_Bidirectional;
        }

    private:
        const NodeType type_;
    };

}