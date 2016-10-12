cmake_minimum_required(VERSION 3.3)
project(hprose)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")

set(SOURCE_FILES
    hprose/io/Writer.cpp)

include_directories(".")

add_library(hprose STATIC ${SOURCE_FILES})

add_executable(WriterTest hprose/io/test/WriterTest.cpp)
target_include_directories(WriterTest PUBLIC "${GTEST_DIR}/include")
target_link_libraries(WriterTest hprose "${GTEST_DIR}/libgtest.a")