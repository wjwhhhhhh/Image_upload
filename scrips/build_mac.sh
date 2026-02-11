#!/bin/bash

# 定义项目名称
PROJECT_NAME="ImageUpload"

# 定义构建目录
BUILD_DIR="build"
DEPS_DIR="deps"

# 定义依赖版本
OPENSSL_VERSION="3.1.4"
POCO_VERSION="1.12.4"

# 检查是否在项目根目录
if [ ! -f "CMakelists.txt" ]; then
    echo "错误：请在项目根目录运行此脚本"
    exit 1
fi

echo "开始构建项目: $PROJECT_NAME"

# 创建依赖目录
mkdir -p "$DEPS_DIR"

# 下载和编译OpenSSL
if [ ! -d "$DEPS_DIR/openssl-${OPENSSL_VERSION}" ]; then
    echo "=== 下载和编译 OpenSSL ==="
    cd "$DEPS_DIR" || exit 1
    
    # 下载OpenSSL源码
    if [ ! -f "openssl-${OPENSSL_VERSION}.tar.gz" ]; then
        echo "下载OpenSSL源码..."
        curl -L -o "openssl-${OPENSSL_VERSION}.tar.gz" "https://www.openssl.org/source/openssl-${OPENSSL_VERSION}.tar.gz"
    fi
    
    # 解压源码
    if [ ! -d "openssl-${OPENSSL_VERSION}" ]; then
        tar -xzf "openssl-${OPENSSL_VERSION}.tar.gz"
    fi
    
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
pwd
rm  $DEPS_DIR/openssl-${OPENSSL_VERSION}.tar.gz

# 下载和编译Poco
if [ ! -d "$DEPS_DIR/poco/" ]; then
    echo "=== 下载和编译 Poco ==="
    mkdir -p "$DEPS_DIR/poco"
    cd "$DEPS_DIR/poco" || exit 1
    
    # 下载Poco源码
    if [ ! -f "poco-${POCO_VERSION}-release.zip" ]; then
        echo "下载Poco源码..."
        curl -L -o "poco-${POCO_VERSION}-release.zip" "https://github.com/pocoproject/poco/archive/refs/tags/poco-${POCO_VERSION}-release.zip"
    fi
    
    # 解压
    unzip -q "poco-${POCO_VERSION}-release.zip"
    rm "poco-${POCO_VERSION}-release.zip"
    
    cd "poco-poco-${POCO_VERSION}-release" || exit 1
    
    # 使用Poco的CMake构建系统，禁用不需要的组件
    mkdir -p cmake-build
    cd cmake-build || exit 1
    
    # 配置Poco（使用正确的CMake选项来省略某些组件）
    cmake .. \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_OSX_ARCHITECTURES="arm64" \
        -DENABLE_NETSSL=ON \
        -DENABLE_CRYPTO=ON \
        -DENABLE_DATA=OFF \
        -DENABLE_DATA_SQLITE=OFF \
        -DENABLE_DATA_MYSQL=OFF \
        -DENABLE_DATA_ODBC=OFF \
        -DENABLE_PDF=OFF \
        -DENABLE_PAGECOMPILER=OFF \
        -DENABLE_PAGECOMPILER_FILE2PAGE=OFF \
        -DENABLE_XML=ON \
        -DENABLE_JSON=ON \
        -DOPENSSL_ROOT_DIR="../../../openssl-${OPENSSL_VERSION}" \
        -DOPENSSL_LIBRARIES="../../../openssl-${OPENSSL_VERSION}/lib" \
        -DOPENSSL_INCLUDE_DIR="../../../openssl-${OPENSSL_VERSION}/include" \
        -DPOCO_UNBUNDLED=ON \
        -DENABLE_TESTS=OFF \
        -DENABLE_SAMPLES=OFF
    
    make -j$(sysctl -n hw.ncpu)
    
    # 确保库文件在预期位置
    if [ ! -d "../lib" ]; then
        mkdir -p "../lib"
    fi
    
    cd ../..
    echo "✅ Poco 编译完成"
else
    echo "✅ Poco 已存在，跳过编译"
fi
