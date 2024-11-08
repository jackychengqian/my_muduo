#!/bin/bash

set -e

# 如果没有build目录，创建该目录
BUILD_DIR="$(pwd)/build"
if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
fi

# 清空build目录
rm -rf "$BUILD_DIR/*"

# 进入build目录，执行cmake和make
cd "$BUILD_DIR"
cmake ..
make

# 回到项目根目录
cd ..

# 把头文件拷贝到 /usr/include/mymuduo，so库拷贝到 /usr/lib
HEADER_DIR="/usr/include/muduo"
LIB_DIR="/usr/lib"

if [ ! -d "$HEADER_DIR" ]; then
    sudo mkdir -p "$HEADER_DIR"
fi

# 拷贝头文件
for header in *.h; do
    sudo cp "$header" "$HEADER_DIR"
done

# 拷贝共享库
cp `pwd`/lib/libmuduo.so /usr/lib

# 刷新动态链接器缓存
sudo ldconfig
