cmake_minimum_required(VERSION 3.26)
project(NetworkingAsn3)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(NetworkingAsn3
        main.cpp
        PostHandler.cpp
        PostHandler.h
        AuthHandler.cpp
        AuthHandler.h
        Server.cpp
        Server.h)

