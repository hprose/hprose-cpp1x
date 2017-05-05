cmake_minimum_required(VERSION 3.2)
project(hprose)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")

set(SOURCE_FILES
        hprose/io/Reader.cpp
        hprose/io/ByteReader.cpp
        hprose/io/RawReader.cpp
        hprose/io/decoders/BoolDecoder.cpp
        hprose/io/decoders/IntDecoder.cpp
        hprose/io/decoders/StringDecoder.cpp
        hprose/rpc/Client.cpp
        hprose/rpc/asio/HttpClient.cpp
        hprose/http/Client.cpp
        hprose/http/asio/Transport.cpp
        hprose/util/Util.cpp
        hprose/Uri.cpp
        hprose/Variant.cpp)

include_directories("/usr/local/include" ".")

add_definitions(-DASIO_STANDALONE)
add_library(hprose STATIC ${SOURCE_FILES})

set(TEST_FILES
        hprose/test/VariantTest.cpp
        hprose/io/test/WriterTest.cpp
        hprose/io/test/ReaderTest.cpp
        hprose/rpc/asio/test/HttpClientTest.cpp)

add_executable(IOTest.out ${TEST_FILES})
target_include_directories(IOTest.out PUBLIC "${GTEST_DIR}/include")
target_link_libraries(IOTest.out hprose pthread "${GTEST_DIR}/libgtest.a")