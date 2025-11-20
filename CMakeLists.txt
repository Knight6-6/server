cmake_minimum_required(VERSION 3.10)

project(server)

set(CMAKE_CXX_STANDARD 23)

set(sources src/client.cpp src/main.cpp src/server.cpp src/message.cpp src/recvserver.cpp src/ringbuf.cpp src/threadpoll.cpp src/usermanager.cpp)

add_executable(server ${sources})

target_include_directories(server PRIVATE include)




