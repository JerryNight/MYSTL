# # 添加测试可执行文件
# add_executable(test_vector test_vector.cpp)
# add_executable(test_basic_string test_basic_string.cpp)

# # 包含 vector 头文件路径
# target_include_directories(test_vector PRIVATE ${PROJECT_SOURCE_DIR})

# # 链接 gtest 库
# target_link_libraries(test_vector gtest gtest_main)

# # 添加测试
# add_test(NAME MyVectorTest COMMAND test_vector)

# test/CMakeLists.txt

# 添加测试子目录，设置变量
cmake_minimum_required(VERSION 3.10)

# 设置测试名称前缀
set(TEST_TARGET_PREFIX test_)

# 添加 GTest
add_subdirectory(${PROJECT_SOURCE_DIR}/../third_party/googletest-1.17.0 ${CMAKE_BINARY_DIR}/gtest)

# 包含 GTest 的头文件路径
include_directories(${PROJECT_SOURCE_DIR}/third_party/googletest-1.17.0/googletest/include)

# 添加你的头文件目录
include_directories(${PROJECT_SOURCE_DIR})

# 枚举所有测试源文件
set(TEST_SOURCES
    #test_vector.cpp
    test_string.cpp
)

# 为每个测试源文件生成一个独立的测试可执行程序
foreach(test_file ${TEST_SOURCES})
    # 获取文件名（不带扩展名）作为 target 名称后缀
    get_filename_component(test_name ${test_file} NAME_WE)
    set(target_name ${TEST_TARGET_PREFIX}${test_name})

    add_executable(${target_name} ${test_file})

    # 链接 GTest 和 pthread（Linux）
    target_link_libraries(${target_name} gtest gtest_main pthread)

    # 添加到 CTest 测试集中（可选）
    add_test(NAME ${target_name} COMMAND ${target_name})
endforeach()
