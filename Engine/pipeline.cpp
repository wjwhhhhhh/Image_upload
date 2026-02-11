#pragma once
#include "./pipeline.h"
#include <iostream>
#include "../NodeTypes/DataFlowCore/FilePathDataFlowCore.h"
using namespace upload;
void Pipeline::start()
{
    std::cout << "Pipeline start" << std::endl;
    std::shared_ptr<INode> filePathDataFlowCore = std::make_shared<FilePathDataFlowCore>();
    filePathDataFlowCore->setConfig("{filepath: \"test.jpg\"}");
    filePathDataFlowCore->Output(std::make_shared<IData>(DataType::DataType_DataRead_Image));
}
void Pipeline::stop()
{
    std::cout << "Pipeline stop" << std::endl;
}
