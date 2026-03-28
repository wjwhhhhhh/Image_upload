#include "./config_manager.h"
#include "./config_contract.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "cJSON.h"

using namespace upload;

ConfigManager::ConfigManager()
{
    loadConfigFromJson("/Users/wangjunwei/Desktop/project/Image_upload/config/config.json", configMap_);
}

bool ConfigManager::loadConfigFromJson(const std::string &jsonFilePath, std::map<ConfigKey, std::string> &configMap)
{
    // 读取 JSON 文件
    std::ifstream file(jsonFilePath);
    if (!file.is_open())
    {
        std::cerr << "[ConfigManager::loadConfigFromJson] Error: Cannot open file " << jsonFilePath << std::endl;
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string jsonContent = buffer.str();
    file.close();

    // 使用 cJSON 解析 JSON
    cJSON *json = cJSON_Parse(jsonContent.c_str());
    if (!json)
    {
        std::cerr << "[ConfigManager::loadConfigFromJson] Error: Invalid JSON format" << std::endl;
        return false;
    }

    bool success = true;

    // 提取 "password" 字段
    cJSON *passwordItem = cJSON_GetObjectItem(json, "AESPassword");
    if (passwordItem && passwordItem->valuestring)
    {
        configMap[ConfigKey::AESPassword] = passwordItem->valuestring;
        std::cout << "[ConfigManager::loadConfigFromJson] Successfully loaded AESPassword" << std::endl;
    }
    else
    {
        std::cerr << "[ConfigManager::loadConfigFromJson] Error: 'password' field not found or invalid" << std::endl;
        success = false;
    }

    // 释放 cJSON 对象
    cJSON_Delete(json);

    return success;
}