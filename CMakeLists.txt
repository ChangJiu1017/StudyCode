cmake_minimum_required(VERSION 3.0) 

project(StudyCode)  

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置源文件目录
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 添加源文件
file(GLOB SOURCES ${SOURCE_DIR}/*.cpp)

# 包含头文件目录
include_directories(${INCLUDE_DIR})

# 添加可执行文件
add_executable(StudyCode ${SOURCES}
        include/std_function.h
        include/call_back_function.h
        include/std_range.h)
