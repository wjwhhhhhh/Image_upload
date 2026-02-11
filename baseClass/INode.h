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
        virtual void Output(std::shared_ptr<IData> data) = 0;
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
            loadInput();
            auto handler = getInputHandler(data);
            if (!handler)
            {
                std::cerr << "No input handler for data type: " << static_cast<int>(data->getType()) << std::endl;
            }
            handler(data);

        }
         void Output(std::shared_ptr<IData> data) final
        {
            if (!supportsOutput())
            {
                throw std::logic_error("Node does not support Output");
            }
            loadOutput();
            auto handler = getOutputHandler(data);
            if (!handler)
            {
                std::cerr << "No output handler for data type: " << static_cast<int>(data->getType()) << std::endl;
                return;
            }
            auto result = handler(data);
            if(OutputDataNode_) {
                OutputDataNode_->Input(result);
            } else {
                // 如果没有下游节点，直接输出结果（用于调试或终端节点）
                std::cout << "Output result generated, but no downstream node to process it" << std::endl;
            }
        }

        void setConfig(const std::string& config)
        {
            config_ = config;
        }
        NodeType getType() const noexcept { return type_; }
        
        // 设置下游节点
        void setOutputNode(std::shared_ptr<INode> node) { OutputDataNode_ = node; }
        
        // 设置上游节点
        void setInputNode(std::shared_ptr<INode> node) { InputDataNode_ = node; }
        
        // 静态方法：连接两个节点
        static bool connectNodes(std::shared_ptr<INode> sourceNode, std::shared_ptr<INode> targetNode)
        {
            if (!sourceNode || !targetNode)
            {
                std::cerr << "Error: Cannot connect null nodes" << std::endl;
                return false;
            }
            
            if (!sourceNode->supportsOutput())
            {
                std::cerr << "Error: Source node does not support output" << std::endl;
                return false;
            }
            
            if (!targetNode->supportsInput())
            {
                std::cerr << "Error: Target node does not support input" << std::endl;
                return false;
            }
            
            // 使用public setter方法设置节点连接
            sourceNode->setOutputNode(targetNode);
            targetNode->setInputNode(sourceNode);
            
            std::cout << "Nodes connected successfully" << std::endl;
            return true;
        }

    protected:

        virtual InputHandler getInputHandler(std::shared_ptr<IData> data) const=0;
        virtual OutputHandler getOutputHandler(std::shared_ptr<IData> data) const=0;
        virtual void loadInput()=0;
        virtual void loadOutput()=0;
        
    public:
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
        std::shared_ptr<INode> InputDataNode_; //上流输入节点
        std::shared_ptr<INode> OutputDataNode_;//下流输出节点
        std::string config_;
        std::shared_ptr<IData> data_;
    };

}