#include "./DataSaveCoreLocalFile.h"
#include "../../DataTypes/ProtectedData/ProtectDataImage.h"
#include "../../manager/config_manager.h"
#include "../../utils/TypeUtils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <cstring>
#include <ctime>
#include <iomanip>
#include "cJSON.h"

namespace upload
{

    void DataSaveCoreLocalFile::loadInput()
    {
        if (DataSaveCoreLocalFile::is_loaded_input_)
        {
            return;
        }
        DataSaveCoreLocalFile::inputHandlers_[DataType::DataType_ProtectedData_Image] = [this](std::shared_ptr<IData> data)
        {
            return onInputImageData(data);
        }; // 用来设置输入处理函数

        DataSaveCoreLocalFile::is_loaded_input_ = true;
    }

    void DataSaveCoreLocalFile::loadOutput()
    {
        if (DataSaveCoreLocalFile::is_loaded_output_)
        {
            return;
        }
    }

    void DataSaveCoreLocalFile::setConfig(const std::string &config)
    {
        // 使用 cJSON 解析配置 JSON 字符串
        cJSON *json = cJSON_Parse(config.c_str());
        if (!json)
        {
            std::cerr << "[DataSaveCoreLocalFile::setConfig] Error: Invalid JSON format" << std::endl;
            return;
        }

        // 提取 "save_dir" 字段
        cJSON *saveDirItem = cJSON_GetObjectItem(json, "save_dir");
        if (saveDirItem && saveDirItem->valuestring)
        {
            save_dir = saveDirItem->valuestring;
            std::cout << "[DataSaveCoreLocalFile::setConfig] save_dir set to: " << save_dir << std::endl;
        }
        else
        {
            std::cerr << "[DataSaveCoreLocalFile::setConfig] Error: 'save_dir' field not found or invalid" << std::endl;
        }

        // 释放 cJSON 对象
        cJSON_Delete(json);
    }
    void DataSaveCoreLocalFile::onInputImageData(std::shared_ptr<IData> data)
    {
        data_ = data;
        std::shared_ptr<ProtectedDataImage> protectedDataImage = std::dynamic_pointer_cast<ProtectedDataImage>(data_);
        if (!protectedDataImage)
        {
            std::cerr << "[DataSaveCoreLocalFile::onInputImageData] Error: Invalid data type." << std::endl;
            return;
        }
        std::string ImageType = getImageStringFromImageFormat(protectedDataImage->format_);

        // 生成带时间戳和随机数的文件名
        auto now = std::time(nullptr);
        auto tm = *std::localtime(&now);
        std::stringstream ss;
        ss << std::put_time(&tm, "%Y%m%d_%H%M%S");
        std::string timestamp = ss.str();

        // 生成 4 个随机数
        unsigned int randomNum = 0;
        if (RAND_bytes(reinterpret_cast<unsigned char *>(&randomNum), sizeof(randomNum)) != 1)
        {
            std::cerr << "[DataSaveCoreLocalFile::onInputImageData] Error: Failed to generate random number" << std::endl;
            return;
        }
        randomNum = randomNum % 10000; // 保留 4 位随机数

        // 组合文件名：encrypted_image_20260308_120530_1234.jpg
        std::stringstream filename;
        filename << save_dir << "encrypted_image_" << timestamp << "_"
                 << std::setfill('0') << std::setw(4) << randomNum << "." << ImageType;
        std::string outputFilePath = filename.str();
        std::ofstream outputFile(outputFilePath, std::ios::binary);
        if (!outputFile.is_open())
        {
            std::cerr << "[DataSaveCoreLocalFile::onInputImageData] Error: Failed to open file for writing: " << outputFilePath << std::endl;
            return;
        }
        outputFile.write(protectedDataImage->buffer_.data(), protectedDataImage->buffer_.size());
        outputFile.close();
        std::cout << "[DataSaveCoreLocalFile::onInputImageData] Successfully saved encrypted image to: " << outputFilePath << std::endl;
    }

} // namespace upload