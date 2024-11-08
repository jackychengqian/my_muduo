cmake_minimum_required(VERSION 3.2)
project(muduo)

#muduo最终编译成so动态库 设置动态库的路径  放在根目录lib文件夹内
set(LIBRARY_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/lib )
#设置调试信息
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ")
set(CMAKE_CXX_STANDARD 17)
#定义参与编译的源代码文件
aux_source_directory(. SRC_LIST)
#编译生成动态库muduo
add_library(muduo SHARED ${SRC_LIST}
        main.cpp)
