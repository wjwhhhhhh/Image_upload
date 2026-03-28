#include "FilePathDataFlowCore.h"
#include "../../deps/cJSON/cJSON.h"
#include "../../DataTypes/DataRead/DataReadImage.h"
#include "../../utils/TypeUtils.h"
using namespace upload;

// 定义静态成员变量
std::map<upload::DataType, upload::InputHandler> upload::FilePathDataFlowCore::inputHandlers_;
std::map<upload::DataType, upload::OutputHandler> upload::FilePathDataFlowCore::outputHandlers_;
bool upload::FilePathDataFlowCore::is_loaded_output_ = false;
bool upload::FilePathDataFlowCore::is_loaded_input_ = false;

std::shared_ptr<IData> FilePathDataFlowCore::onOutputImageFile(std::shared_ptr<IData> data)
{
    std::cout << "filePathDataFlowCore onOutputImageFile" << std::endl;
    cJSON* json = cJSON_Parse(config_.c_str());
    std::cout << "config_: " << config_ << std::endl;
    if (!json)
    {
        std::cerr << "Error parsing JSON: " << cJSON_GetErrorPtr() << std::endl;
        return nullptr;
    }
    cJSON* filepath = cJSON_GetObjectItemCaseSensitive(json, "filepath");
    if (!filepath || !cJSON_IsString(filepath))
    {
        std::cerr << "Error: filepath is not a string in JSON" << std::endl;
        cJSON_Delete(json);
        return nullptr;
    }
    std::string filepathStr = filepath->valuestring;
    std::cout << "filepathStr: " << filepathStr << std::endl;
    cJSON_Delete(json);

    return std::make_shared<DataReadImage>(filepathStr, getImageFormatFromFilePath(filepathStr));
}
