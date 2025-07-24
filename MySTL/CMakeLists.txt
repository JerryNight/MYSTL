cmake_minimum_required(VERSION 3.10)
project(MySTLTest)

# 使用 C++17 或更高版本
set(CMAKE_CXX_STANDARD 17)

# debug
set(CMAKE_BUILD_TYPE Debug)


# 添加子目录
add_subdirectory(third_party/googletest-1.17.0)
enable_testing()
add_subdirectory(test)
