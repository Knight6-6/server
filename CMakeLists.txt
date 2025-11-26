cmake_minimum_required(VERSION 3.10)

project(server)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(base)
add_subdirectory(protocol)
add_subdirectory(utils)
add_subdirectory(business)
add_subdirectory(core)

add_executable(server main.cpp)

target_include_directories(server PUBLIC core)

target_link_libraries(server PUBLIC core_lib)