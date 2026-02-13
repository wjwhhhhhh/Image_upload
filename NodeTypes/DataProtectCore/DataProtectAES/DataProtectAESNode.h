#pragma once
#include "../DataProtectCoreBase.h"
namespace upload {
    class DataProtectAESNode : public DataProtectCoreBase {
    public:
        DataProtectAESNode() : DataProtectCoreBase() {}
        virtual ~DataProtectAESNode() = default;
        void loadInput() override;
        void loadOutput() override;
    private:
        void onInputImageData(std::shared_ptr<IData> data);
        std::shared_ptr<IData> onOutputImageData(std::shared_ptr<IData> data);
    };
} // namespace upload