cmake_minimum_required(VERSION 3.10)
project(Kaleidoscope)

option(ENABLE_TEST "Enable test" ON)

# 设置编译选项
set(CMAKE_BUILD_TYPE Debug)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")

include_directories(${CMAKE_SOURCE_DIR}/include)
# 查找所有源文件和头文件
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)

# 添加LLVM依赖
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# 添加LLVM头文件路径
include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

add_library(kaleidoscope_lib STATIC ${SOURCES})

# 链接LLVM库
llvm_map_components_to_libnames(llvm_libs all)
target_link_libraries(kaleidoscope_lib ${llvm_libs})

# 添加可执行文件
add_executable(kaleidoscope ${CMAKE_SOURCE_DIR}/tools/main.cpp ${SOURCES})

if(ENABLE_TEST)
    add_subdirectory(test)
endif()
