cmake_minimum_required(VERSION 2.8.4)
project(memread)

set(SOURCE_FILES main.cpp)
add_executable(memread ${SOURCE_FILES})
add_definitions(-std=c++11)
SET(CMAKE_CXX_FLAGS "-std=c++0x")
SET(CMAKE_CXX_FLAGS "-pthread")