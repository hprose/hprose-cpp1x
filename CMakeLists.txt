cmake_minimum_required(VERSION 3.3)
project(hprose)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    hprose/io/HproseWriter.cpp)

include_directories(".")

add_library(hprose STATIC ${SOURCE_FILES})