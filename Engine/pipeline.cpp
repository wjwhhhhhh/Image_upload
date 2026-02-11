#pragma once
#include "./pipeline.h"
#include <iostream>
#include "../NodeTypes/DataFlowCore/FilePathDataFlowCore.h"
#include "../NodeTypes/DataReadCore/ImageDataRead.h"
using namespace upload;
void Pipeline::start()
{
    std::cout << "Pipeline start" << std::endl;
    std::shared_ptr<INode> filePathDataFlowCore = std::make_shared<FilePathDataFlowCore>();
    std::shared_ptr<INode> imageDataRead = std::make_shared<ImageDataRead>();
    if (!INode::connectNodes(filePathDataFlowCore, imageDataRead))
    {
        std::cerr << "Error: Failed to connect nodes" << std::endl;
        return;
    }
    filePathDataFlowCore->setConfig("{\"filepath\": \"/Users/wangjunwei/Desktop/project/Image_upload/test.jpg\"}");
    filePathDataFlowCore->Output(std::make_shared<IData>(DataType::DataType_DataFlow_ImageFile));
    imageDataRead->setConfig("");
    imageDataRead->Output(std::make_shared<IData>(DataType::DataType_OriginalData_Image));
}

void Pipeline::stop()
{
    std::cout << "Pipeline stop" << std::endl;
}
