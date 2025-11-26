cmake_minimum_required(VERSION 3.10)

project(utils)

set(CMAKE_CXX_STANDARD 20)

add_library(utils_lib logger.cpp object_poll.cpp thread_poll.cpp ring_buf.cpp session.cpp)

target_include_directories(utils_lib PUBLIC ../base)

target_link_libraries(utils_lib PUBLIC base_lib)
