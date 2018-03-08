cmake_minimum_required(VERSION 2.8)

project(raytraycer)
# Debug选项
set(CMAKE_C_FLAGS_DEBUG "-O0 -Wall -g -ggdb -std=c++11")
# Release选项
set(CMAKE_C_FLAGS_RELEASE "-O3 -Wall -std=c++11")

include_directories(./lib/eigen/)

aux_source_directory(./src SRC_DIR)

add_executable(raytracer ${SRC_DIR})

set(CMAKE_CXX_COMPILER      "clang++" )         # 显示指定使用的C++编译器
set(CMAKE_CXX_FLAGS   "-std=c++11 -Wall -O2")             # c++11
set(CMAKE_CXX_FLAGS_DEBUG   "-O0" )             # 调试包不优化
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG " )   # release包优化