#pragma once
#include "../DataProtectCoreBase.h"
#include <string>
#include <vector>
namespace upload
{
    class DataProtectAESNode : public DataProtectCoreBase
    {
    public:
        DataProtectAESNode() : DataProtectCoreBase() {}
        virtual ~DataProtectAESNode() = default;
        void loadInput() override;
        void loadOutput() override;

    private:
        void onInputImageData(std::shared_ptr<IData> data);
        std::shared_ptr<IData> onOutputImageData(std::shared_ptr<IData> data);

        // AES256 encryption helper methods
        std::vector<unsigned char> deriveKeyFromPassword(const std::string &password);
        std::vector<unsigned char> encryptAES256(const std::vector<char> &plain_data,
                                                 const std::vector<unsigned char> &key);
    };
} // namespace upload