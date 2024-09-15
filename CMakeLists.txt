cmake_minimum_required(VERSION 3.10)
project(MyLibrary C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(linuxmath SHARED src/linux_math.c)

include_directories(${PROJECT_SOURCE_DIR}/include)

target_include_directories(linuxmath PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(main ${PROJECT_SOURCE_DIR}/src/main.c)
target_link_libraries(main linuxmath)