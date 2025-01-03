cmake_minimum_required(VERSION 3.29)
project(tiny_net)

set(CMAKE_CXX_STANDARD 20)

add_executable(tiny_net src/main.cpp
        src/tap.cpp
        src/utils.cpp)

target_include_directories(tiny_net PRIVATE src)
