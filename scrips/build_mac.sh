#!/bin/bash

# 定义项目名称
PROJECT_NAME="ImageUpload"

# 定义构建目录
BUILD_DIR="build"
DEPS_DIR="deps"

# 定义依赖版本
OPENSSL_VERSION="3.1.4"
POCO_VERSION="1.12.4"
NEED_BUILD_OPENSSL="FALSE"
NEED_BUILD_POCO="TRUE"

# 检查是否在项目根目录
if [ ! -f "CMakelists.txt" ]; then
    echo "错误：请在项目根目录运行此脚本"
    exit 1
fi

echo "开始构建项目: $PROJECT_NAME"

# 创建依赖目录
mkdir -p "$DEPS_DIR"

# 下载和编译OpenSSL
if [ "$NEED_BUILD_OPENSSL" = TRUE ]; then
    cd "$DEPS_DIR" || exit 1
    rm -rf "openssl"
    mkdir -p "openssl"
    echo "=== 下载和编译 OpenSSL ==="
    cd "openssl" || exit 1
    
    # 下载OpenSSL源码
    if [ ! -f "openssl-${OPENSSL_VERSION}.tar.gz" ]; then
        echo "下载OpenSSL源码..."
        curl -L -o "openssl-${OPENSSL_VERSION}.tar.gz" "https://www.openssl.org/source/openssl-${OPENSSL_VERSION}.tar.gz"
    fi
    
    # 解压源码
    if [ ! -d "openssl-${OPENSSL_VERSION}" ]; then
        tar -xzf "openssl-${OPENSSL_VERSION}.tar.gz"
    fi
    
    rm -rf "openssl-${OPENSSL_VERSION}.tar.gz"
    cd "openssl-${OPENSSL_VERSION}" || exit 1
    
    # 创建构建目录
    mkdir -p build
    cd build || exit 1
    
    # 配置和编译OpenSSL
    ../Configure darwin64-arm64-cc no-shared no-zlib no-async no-comp --prefix="$(pwd)/.." --openssldir="$(pwd)/../ssl"
    make -j$(sysctl -n hw.ncpu)
    
    # 确保库文件在预期位置
    if [ ! -d "../lib" ]; then
        mkdir -p "../lib"
    fi
    if [ -f "libcrypto.a" ]; then
        cp libcrypto.a ../lib/
    fi
    if [ -f "libssl.a" ]; then
        cp libssl.a ../lib/
    fi
    
    cd ../..
    echo "✅ OpenSSL 编译完成"
else
    echo "✅ OpenSSL 已存在，跳过编译"
fi
cd ..

# 下载和编译Poco
if [ "$NEED_BUILD_POCO" = TRUE ]; then
    echo "=== 下载和编译 Poco ==="
    mkdir -p "poco"
    cd "poco" || exit 1
    
    # 下载Poco源码
    if [ ! -f "poco-${POCO_VERSION}-release.zip" ]; then
        echo "下载Poco源码..."
        curl -L -o "poco-${POCO_VERSION}-release.zip" "https://github.com/pocoproject/poco/archive/refs/tags/poco-${POCO_VERSION}-release.zip"
    fi
    
    # 解压
    unzip -q "poco-${POCO_VERSION}-release.zip"
    rm "poco-${POCO_VERSION}-release.zip"
    
    cd "poco-poco-${POCO_VERSION}-release" || exit 1
    
    # 使用Poco的原生配置系统，指定OpenSSL路径
    ./configure --omit=Data/MySQL,Data/ODBC \
                --config=Darwin64 \
                --prefix="$(pwd)" \
    
    # 编译
    make -s -j$(sysctl -n hw.ncpu)
    
    # 安装到本地目录
    make -s install
    
    cd ../..
    echo "✅ Poco 编译完成"
else
    echo "✅ Poco 已存在，跳过编译"
fi
