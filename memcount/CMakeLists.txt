cmake_minimum_required(VERSION 2.8.4)
project(memcount)

set(SOURCE_FILES main.cpp)
add_executable(memcount ${SOURCE_FILES})
add_definitions(-std=c++11)
SET(CMAKE_CXX_FLAGS "-std=c++0x")