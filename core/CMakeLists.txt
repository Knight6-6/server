cmake_minimum_required(VERSION 3.10)

project (core)

set(CMAKE_CXX_STANDARD 20)

add_library(core_lib ioserver.cpp server.cpp)

target_include_directories(core_lib PUBLIC ../business ../protocol ../utils)

target_link_libraries(core_lib PUBLIC business_lib protocol_lib utils_lib)