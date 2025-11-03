cmake_minimum_required(VERSION 3.10)

project(server)

set(CMAKE_CXX_STANDARD 11)

set(sources src/client.cpp src/main.cpp src/server.cpp)

add_executable(server ${sources})

target_include_directories(server PRIVATE include)




