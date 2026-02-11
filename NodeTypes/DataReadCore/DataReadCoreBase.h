#pragma once
#include "../../baseClass/INode.h"
namespace upload
{
    //未来可能要爬虫读取数据
    class DataReadCoreBase : public INode
    {
    public:
        DataReadCoreBase(NodeType type) : INode(type) {}
        virtual ~DataReadCoreBase() = default;
    };
} // namespace upload
