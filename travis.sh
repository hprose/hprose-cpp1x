#!/usr/bin/env sh

curl -sSLo googletest-release-1.8.0.tar.gz https://github.com/google/googletest/archive/release-1.8.0.tar.gz
tar xzvf googletest-release-1.8.0.tar.gz
cd googletest-release-1.8.0/googletest && cmake . && make && cd .. && cd ..
cmake -DGTEST_DIR=`pwd`/googletest-release-1.8.0/googletest && make && ./WriterTest
