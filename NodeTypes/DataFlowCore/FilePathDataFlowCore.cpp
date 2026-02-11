#include "FilePathDataFlowCore.h"
#include "../../deps/cJSON/cJSON.h"
using namespace upload;
std::shared_ptr<IData> FilePathDataFlowCore::onOutputImageFile(std::shared_ptr<IData> data)
{
    //std::string config=""{filepath: \"test.jpg\"}"";
    cJSON* json = cJSON_Parse(config_.c_str());
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
    cJSON_Delete(json);
    return std::make_shared<IData>(DataType::DataType_DataRead_Image, filepathStr);
}
