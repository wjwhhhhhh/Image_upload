#pragma once
#include <map>
#include <string>
#include <iostream>
#include "./config_contract.h"
namespace upload
{
    class ConfigManager
    {
    public:
        static ConfigManager &getInstance()
        {
            static ConfigManager instance;
            return instance;
        }

        // 从 JSON 文件读取配置并填充 map
        bool loadConfigFromJson(const std::string &jsonFilePath, std::map<ConfigKey, std::string> &configMap);
        std::string getConfig(ConfigKey key) const
        {
            auto it = configMap_.find(key);
            if (it != configMap_.end())
            {
                return it->second;
            }
            else
            {
                std::cerr << "[ConfigManager::getConfig] Warning: Config key not found" << std::endl;
                return "";
            }
        }

    private:
        ConfigManager();
        ~ConfigManager() = default;
        ConfigManager(const ConfigManager &) = delete;
        ConfigManager &operator=(const ConfigManager &) = delete;
        std::map<ConfigKey, std::string> configMap_;
    };

}