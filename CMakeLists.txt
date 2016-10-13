cmake_minimum_required(VERSION 3.2)
project(hprose)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")
if (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang" AND ${CMAKE_SYSTEM} MATCHES "Linux")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -lc++abi")
endif()

set(SOURCE_FILES
    hprose/io/Writer.cpp
    hprose/util/Util.cpp)

include_directories(".")

add_library(hprose STATIC ${SOURCE_FILES})

add_executable(WriterTest hprose/io/test/WriterTest.cpp)
target_include_directories(WriterTest PUBLIC "${GTEST_DIR}/include")
target_link_libraries(WriterTest hprose pthread "${GTEST_DIR}/libgtest.a")