#include "./DataProtectAESNode.h"
#include "../../DataTypes/OriginalData/OriginalDataImages.h"
#include "../../DataTypes/ProtectedData/ProtectDataImage.h"
#include "../../manager/config_manager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <cstring>

namespace upload
{

    void DataProtectAESNode::loadInput()
    {
        if (DataProtectAESNode::is_loaded_input_)
        {
            return;
        }
        DataProtectAESNode::inputHandlers_[DataType::DataType_OriginalData_Image] = [this](std::shared_ptr<IData> data)
        {
            return onInputImageData(data);
        }; // 用来设置输入处理函数

        DataProtectAESNode::is_loaded_input_ = true;
    }

    void DataProtectAESNode::loadOutput()
    {
        if (DataProtectAESNode::is_loaded_output_)
        {
            return;
        }

        DataProtectAESNode::outputHandlers_[DataType::DataType_ProtectedData_Image] = [this](std::shared_ptr<IData> data)
        {
            return onOutputImageData(data);
        }; // 用来设置输出处理函数

        DataProtectAESNode::is_loaded_output_ = true;
    }

    void DataProtectAESNode::onInputImageData(std::shared_ptr<IData> data)
    {
        data_ = data;
    }

    std::shared_ptr<IData> DataProtectAESNode::onOutputImageData(std::shared_ptr<IData> data)
    {
        std::shared_ptr<OriginalDataImage> originalDataImage = std::dynamic_pointer_cast<OriginalDataImage>(data_);
        if (!originalDataImage)
        {
            std::cerr << "[DataProtectAESNode::onOutputImageData] Error: Invalid data type." << std::endl;
            return nullptr;
        }

        // 从配置中获取密码
        std::string password = ConfigManager::getInstance().getConfig(ConfigKey::AESPassword);
        if (password.empty())
        {
            std::cerr << "[DataProtectAESNode::onOutputImageData] Error: Failed to get password from config" << std::endl;
            return nullptr;
        }

        // 从密码派生加密密钥（PBKDF2）
        std::vector<unsigned char> key = deriveKeyFromPassword(password);
        if (key.size() != 32)
        {
            std::cerr << "[DataProtectAESNode::onOutputImageData] Error: Failed to derive encryption key" << std::endl;
            return nullptr;
        }

        // 使用 AES-256-CBC 加密数据
        std::vector<unsigned char> encryptedData = encryptAES256(originalDataImage->buffer_, key);
        if (encryptedData.empty())
        {
            std::cerr << "[DataProtectAESNode::onOutputImageData] Error: Encryption failed" << std::endl;
            return nullptr;
        }

        std::cout << "[DataProtectAESNode::onOutputImageData] Successfully encrypted image data using AES-256. "
                  << "Original size: " << originalDataImage->buffer_.size()
                  << " bytes, Encrypted size: " << encryptedData.size() << " bytes" << std::endl;

        // 创建受保护数据对象并返回
        std::vector<char> encryptedBuffer(encryptedData.begin(), encryptedData.end());
        auto protectedData = std::make_shared<ProtectedDataImage>(encryptedBuffer, originalDataImage->format_);

        return protectedData;
    }

    std::vector<unsigned char> DataProtectAESNode::deriveKeyFromPassword(const std::string &password)
    {
        std::vector<unsigned char> key(32); // 256 bits = 32 bytes
        unsigned char salt[16] = {0};       // In production, use random salt and store it

        // 使用 PBKDF2 和 SHA256 派生密钥
        int iterations = 100000;
        if (!PKCS5_PBKDF2_HMAC(password.c_str(), password.length(),
                               salt, sizeof(salt),
                               iterations,
                               EVP_sha256(),
                               32, key.data()))
        {
            std::cerr << "[DataProtectAESNode::deriveKeyFromPassword] Error: Failed to derive key" << std::endl;
            return std::vector<unsigned char>(32, 0);
        }

        return key;
    }

    std::vector<unsigned char> DataProtectAESNode::encryptAES256(const std::vector<char> &plain_data,
                                                                 const std::vector<unsigned char> &key)
    {
        EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
        if (!ctx)
        {
            std::cerr << "[DataProtectAESNode::encryptAES256] Error: Failed to create cipher context" << std::endl;
            return std::vector<unsigned char>();
        }

        // 生成随机 IV (16 bytes for AES)
        std::vector<unsigned char> iv(EVP_MAX_IV_LENGTH);
        if (RAND_bytes(iv.data(), iv.size()) != 1)
        {
            std::cerr << "[DataProtectAESNode::encryptAES256] Error: Failed to generate random IV" << std::endl;
            EVP_CIPHER_CTX_free(ctx);
            return std::vector<unsigned char>();
        }

        // 初始化加密
        if (!EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), iv.data()))
        {
            std::cerr << "[DataProtectAESNode::encryptAES256] Error: Failed to initialize encryption" << std::endl;
            EVP_CIPHER_CTX_free(ctx);
            return std::vector<unsigned char>();
        }

        // 分配输出缓冲区
        std::vector<unsigned char> ciphertext(plain_data.size() + EVP_MAX_BLOCK_LENGTH);
        int len = 0;
        int ciphertext_len = 0;

        // 加密数据
        if (!EVP_EncryptUpdate(ctx, ciphertext.data(), &len,
                               reinterpret_cast<const unsigned char *>(plain_data.data()),
                               plain_data.size()))
        {
            std::cerr << "[DataProtectAESNode::encryptAES256] Error: Encryption failed" << std::endl;
            EVP_CIPHER_CTX_free(ctx);
            return std::vector<unsigned char>();
        }
        ciphertext_len = len;

        // 完成加密
        if (!EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len))
        {
            std::cerr << "[DataProtectAESNode::encryptAES256] Error: Encryption finalization failed" << std::endl;
            EVP_CIPHER_CTX_free(ctx);
            return std::vector<unsigned char>();
        }
        ciphertext_len += len;

        EVP_CIPHER_CTX_free(ctx);

        // 将 IV 和密文合并（IV 需要随密文一起传输以便解密）
        std::vector<unsigned char> result;
        result.insert(result.end(), iv.begin(), iv.end());
        result.insert(result.end(), ciphertext.begin(), ciphertext.begin() + ciphertext_len);

        return result;
    }
} // namespace upload