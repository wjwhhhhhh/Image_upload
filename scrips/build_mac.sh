#!/bin/bash

# 定义项目名称
PROJECT_NAME="ImageUpload"

# 定义构建目录
BUILD_DIR="build"
DEPS_DIR="deps"

# 定义依赖版本
OPENSSL_VERSION="3.1.4"
POCO_VERSION="1.9.4"
NEED_BUILD_OPENSSL="FALSE"
NEED_BUILD_POCO="FALSE"
NEED_BUILD_COSSDK="TRUE"

PROJECT_ROOT_DIR=$(pwd)

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
    
    cd ../../..
    echo "✅ OpenSSL 编译完成"
else
    echo "✅ OpenSSL 已存在，跳过编译"
fi

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
    
    # 解压（静默模式，自动覆盖）
    unzip -q -o "poco-${POCO_VERSION}-release.zip"
    rm "poco-${POCO_VERSION}-release.zip"
    
    cd "poco-poco-${POCO_VERSION}-release" || exit 1
    
    # 创建构建目录
    mkdir -p cmake-build
    cd cmake-build || exit 1
    
    # 计算OpenSSL的绝对路径
    OPENSSL_PATH="$(cd ../../../../deps/openssl/openssl-${OPENSSL_VERSION} && pwd)"
    
    # 修复 zlib 与新版本 Clang 的兼容性问题
    # 注释掉有问题的 fdopen 宏定义
    sed -i '' '/#.*define fdopen(fd,mode)/s/.*/# define fdopen_disabled/' ../Foundation/src/zutil.h 2>/dev/null || true
    
    # 使用CMake配置Poco为静态库，链接到本地OpenSSL
    # 使用绝对路径确保能找到OpenSSL
    cmake .. \
        -DCMAKE_POLICY_VERSION_MINIMUM=3.10 \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_OSX_ARCHITECTURES="arm64" \
        -DCMAKE_C_FLAGS="-Wno-error=incompatible-function-pointer-types -Wno-error" \
        -DCMAKE_CXX_FLAGS="-Wno-error" \
        -DOPENSSL_ROOT_DIR="${OPENSSL_PATH}" \
        -DOPENSSL_LIB_DIR="${OPENSSL_PATH}/lib" \
        -DOPENSSL_INCLUDE_DIR="${OPENSSL_PATH}/include" \
        -DOPENSSL_CRYPTO_LIBRARY="${OPENSSL_PATH}/lib/libcrypto.a" \
        -DOPENSSL_SSL_LIBRARY="${OPENSSL_PATH}/lib/libssl.a" \
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
        -DENABLE_ZIP=OFF \
        -DPOCO_UNBUNDLED=OFF \
        -DENABLE_TESTS=OFF \
        -DENABLE_SAMPLES=OFF \
        -DBUILD_SHARED_LIBS=OFF \
        -DCMAKE_INSTALL_PREFIX="$(pwd)/../../poco-static"
    
    # 编译静态库
    if ! cmake --build . --config Release -j$(sysctl -n hw.ncpu); then
        echo "⚠️  Poco 构建有错误，继续尝试安装已生成的文件..."
    fi
    
    # 安装到本地目录（即使有部分失败也继续）
    cmake --install . 2>/dev/null || true
    
    # 验证必要的库是否存在
    if [ ! -d "../../poco-static/lib/cmake/Poco" ]; then
        echo "⚠️  警告：Poco 构建可能不完整，但继续..."
    fi
    
    
else
    echo "✅ Poco 已存在，跳过编译"
fi

cd PROJECT_ROOT_DIR || exit 1
cd DEPS_DIR || exit 1
if [ "$NEED_BUILD_COSSDK" = TRUE ]; then
    echo "=== 下载和编译 COSSDK ==="
    mkdir -p "cossdk"
    cd "cossdk" || exit 1
    
    # 克隆 COSSDK 仓库
    if [ ! -d "cos-cpp-sdk-v5-5.5.19" ]; then
        echo "克隆 COSSDK 源码..."
        wget https://github.com/tencentyun/cos-cpp-sdk-v5/archive/refs/tags/v5.5.19.zip
        unzip v5.5.19.zip
        rm v5.5.19.zip
    fi
    cd "cos-cpp-sdk-v5-5.5.19" || exit 1
    mkdir build 
    cd build 
else 
    echo "✅ COSSDK 已存在，跳过编译"
fi

