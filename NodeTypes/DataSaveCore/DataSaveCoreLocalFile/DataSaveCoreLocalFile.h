#pragma once
#include "../DataSaveCoreBase.h"
#include <string>
#include <vector>
namespace upload
{
    class DataSaveCoreLocalFile : public DataSaveCoreBase
    {
    public:
        DataSaveCoreLocalFile() : DataSaveCoreBase() {}
        virtual ~DataSaveCoreLocalFile() = default;
        void loadInput() override;
        void loadOutput() override;
        void setConfig(const std::string &config) override;
        
    private:
        void onInputImageData(std::shared_ptr<IData> data);
        std::shared_ptr<IData> onOutputImageData(std::shared_ptr<IData> data);
        std::string save_dir;
    };
} // namespace upload