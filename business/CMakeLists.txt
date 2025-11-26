cmake_minimum_required(VERSION 3.10)

project (business)

set(CMAKE_CXX_STANDARD 20)

add_library(business_lib user_manager.cpp)

target_include_directories(business_lib PUBLIC ../protocol ../utils)

target_link_libraries(business_lib PUBLIC protocol_lib utils_lib)