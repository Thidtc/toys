cmake_minimum_required (VERSION 2.8)
project (cjson)

set(CMAKE_C_FLAGS_DEBUG "-O0 -Wall -g -ggdb")
set(CMAKE_C_FLAGS_RELEASE "-O3 -Wall")

add_library(cjson cjson.c)
add_executable(cjson_test test.c)
target_link_libraries(cjson_test cjson)
