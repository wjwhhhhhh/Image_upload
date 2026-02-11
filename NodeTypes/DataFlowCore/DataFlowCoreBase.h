#pragma once
#include "../../baseClass/INode.h"
namespace upload {
// DataFlowCoreBase.h
// 这个类就是所有流水线的起点
class DataFlowCoreBase : public INode {
public:
    DataFlowCoreBase(NodeType type) : INode(type) {}
    virtual ~DataFlowCoreBase() = default;
};
} // namespace upload