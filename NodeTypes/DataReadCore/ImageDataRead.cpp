#include "ImageDataRead.h"
#include "../../DataTypes/DataRead/DataReadImage.h"
#include "../../DataTypes/OriginalData/OriginalDataImages.h"
#include <fstream>
#include <vector>
using namespace upload;

// 定义静态成员变量
std::map<upload::DataType, upload::InputHandler> upload::ImageDataRead::inputHandlers_;
std::map<upload::DataType, upload::OutputHandler> upload::ImageDataRead::outputHandlers_;
bool upload::ImageDataRead::is_loaded_input_ = false;
bool upload::ImageDataRead::is_loaded_output_ = false;

std::shared_ptr<IData> ImageDataRead::onOutputImageFile(std::shared_ptr<IData> data)
{
    std::cout << "imageDataRead onOutputImageFile" << std::endl;
    if (data_ == nullptr) {
        std::cerr << "Error: Data is nullptr" << std::endl;
        return nullptr;
    }
    auto imageData = std::dynamic_pointer_cast<DataReadImage>(data_);
    if (!imageData)
    {
        std::cerr << "Error: data is not a DataReadImage" << std::endl;
        return nullptr;
    }
    std::string filepath = imageData->filepath_;
    
    // 读取图像文件
    std::cout << "Reading image file: " << filepath << std::endl;
    
    // 检查文件是否存在
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error: Cannot open file: " << filepath << std::endl;
        return nullptr;
    }
    
    // 获取文件大小
    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    
    if (fileSize == 0)
    {
        std::cerr << "Error: File is empty: " << filepath << std::endl;
        return nullptr;
    }
    
    // 读取文件内容到缓冲区
    std::vector<char> buffer(fileSize);
    file.read(buffer.data(), fileSize);
    file.close();
    
    std::cout << "Successfully read image file: " << filepath 
              << " (size: " << fileSize << " bytes)" << std::endl;
    
    // 返回包含图像数据的对象（这里需要根据您的数据结构来调整）
    // 目前先返回原始的DataReadImage对象
    auto originalImageData = std::make_shared<OriginalDataImage>(std::move(buffer),imageData->format_);
    return originalImageData;
}
void ImageDataRead::onInputImageFile(std::shared_ptr<IData> data)
{
    std::cout << "onInputImageFile" << std::endl;
    data_ = data;
}
