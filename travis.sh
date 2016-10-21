#!/usr/bin/env sh

cmake -DGTEST_DIR=`pwd`/googletest-release-1.8.0/googletest && make && ./IOTest.out
