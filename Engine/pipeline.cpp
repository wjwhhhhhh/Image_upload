#pragma once
#include "./pipeline.h"
#include <iostream>
#include "../NodeTypes/DataFlowCore/FilePathDataFlowCore.h"
#include "../NodeTypes/DataReadCore/ImageDataRead.h"
#include "../NodeTypes/DataProtectCore/DataProtectAES/DataProtectAESNode.h"
#include "../NodeTypes/DataSaveCore/DataSaveCoreLocalFile/DataSaveCoreLocalFile.h"
using namespace upload;
void Pipeline::start()
{
    std::cout << "Pipeline start" << std::endl;
    std::shared_ptr<INode> filePathDataFlowCore = std::make_shared<FilePathDataFlowCore>();
    std::shared_ptr<INode> imageDataRead = std::make_shared<ImageDataRead>();
    std::shared_ptr<INode> dataProtectAESNode = std::make_shared<DataProtectAESNode>();
    std::shared_ptr<INode> dataSaveCoreLocalFile = std::make_shared<DataSaveCoreLocalFile>();
    dataSaveCoreLocalFile->setConfig("{\"save_dir\": \"/Users/wangjunwei/Desktop/project/Image_upload/output/\"}");
    if (!INode::connectNodes(filePathDataFlowCore, imageDataRead))
    {
        std::cerr << "Error: Failed to connect nodes" << std::endl;
        return;
    }
    if (!INode::connectNodes(imageDataRead, dataProtectAESNode))
    {
        std::cerr << "Error: Failed to connect nodes" << std::endl;
        return;
    }
    if (!INode::connectNodes(dataProtectAESNode, dataSaveCoreLocalFile))
    {
        std::cerr << "Error: Failed to connect nodes" << std::endl;
        return;
    }
    filePathDataFlowCore->setConfig("{\"filepath\": \"/Users/wangjunwei/Desktop/project/Image_upload/test.jpg\"}");
    filePathDataFlowCore->Output(std::make_shared<IData>(DataType::DataType_DataFlow_ImageFile));
    imageDataRead->setConfig("");
    imageDataRead->Output(std::make_shared<IData>(DataType::DataType_OriginalData_Image));
    dataProtectAESNode->setConfig("");
    dataProtectAESNode->Output(std::make_shared<IData>(DataType::DataType_ProtectedData_Image));
}

void Pipeline::stop()
{
    std::cout << "Pipeline stop" << std::endl;
}
