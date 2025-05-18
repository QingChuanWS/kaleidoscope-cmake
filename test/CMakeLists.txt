enable_testing()

# 编译 GoogleTest（会根据源码生成链接库等产物）
add_subdirectory(${CMAKE_SOURCE_DIR}/third-party/gtest ${CMAKE_BINARY_DIR}/third-party/gtest)

# 设置测试源码文件
set(TEST_SOURCES
    test_lexer.cpp
)

# 创建测试可执行文件
add_executable(kaleidoscope_tests ${TEST_SOURCES})

# 链接被测库、GoogleTest 和 pthread (CMAKE_THREAD_LIBS_INIT 为线程库)
target_link_libraries(kaleidoscope_tests PRIVATE kaleidoscope_lib gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})

# 注册测试可执行文件到 CTest
add_test(NAME KaleidoscopeTests COMMAND kaleidoscope_tests)