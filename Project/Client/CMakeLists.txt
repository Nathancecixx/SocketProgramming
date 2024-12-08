cmake_minimum_required(VERSION 3.26)
project(Client)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Client
        main.cpp
        postMessage.cpp
        postMessage.h
        viewMessage.cpp
        viewMessage.h)

