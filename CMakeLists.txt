cmake_minimum_required(VERSION 3.2)
project(hprose)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
if (CMAKE_COMPILER_IS_GNUCXX)
    #gcc
    execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion
                    OUTPUT_VARIABLE COMPILER_VERSION)
    if (COMPILER_VERSION VERSION_GREATER 4.7 OR COMPILER_VERSION VERSION_EQUAL 4.7)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    endif()
endif ()
if (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif ()

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