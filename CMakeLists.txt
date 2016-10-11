cmake_minimum_required(VERSION 3.3)
project(hprose)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")

set(SOURCE_FILES
    hprose/io/Writer.cpp)

include_directories(".")

add_library(hprose STATIC ${SOURCE_FILES})

add_executable(HproseWriterTest hprose/io/test/WriterTest.cpp)
target_include_directories(HproseWriterTest PUBLIC "${GTEST_DIR}/include")
target_link_libraries(HproseWriterTest hprose "${GTEST_DIR}/libgtest.a")