# 添加测试可执行文件
add_executable(test_vector test_vector.cpp)

# 包含 vector 头文件路径
target_include_directories(test_vector PRIVATE ${PROJECT_SOURCE_DIR})

# 链接 gtest 库
target_link_libraries(test_vector gtest gtest_main)

# 添加测试
add_test(NAME MyVectorTest COMMAND test_vector)
