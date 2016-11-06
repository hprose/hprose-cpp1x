cmake_minimum_required(VERSION 3.2)
project(hprose)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")
if (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang" AND ${CMAKE_SYSTEM} MATCHES "Linux")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/include/c++/v1 -stdlib=libc++ -lc++abi")
endif()

set(SOURCE_FILES
    hprose/io/Writer.cpp
    hprose/io/Reader.cpp
    hprose/io/decoders/BoolDecoder.cpp
    hprose/io/decoders/IntDecoder.cpp
    hprose/util/Util.cpp
    hprose/Variant.cpp)

include_directories(".")

add_library(hprose STATIC ${SOURCE_FILES})

add_executable(IOTest.out hprose/io/test/WriterTest.cpp hprose/io/test/ReaderTest.cpp)
target_include_directories(IOTest.out PUBLIC "${GTEST_DIR}/include")
target_link_libraries(IOTest.out hprose pthread "${GTEST_DIR}/libgtest.a")