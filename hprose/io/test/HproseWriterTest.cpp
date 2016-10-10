/**********************************************************\
|                                                          |
|                          hprose                          |
|                                                          |
| Official WebSite: http://www.hprose.com/                 |
|                   http://www.hprose.org/                 |
|                                                          |
\**********************************************************/

/**********************************************************\
 *                                                        *
 * HproseWriterTest.cpp                                   *
 *                                                        *
 * hprose writer test for cpp.                            *
 *                                                        *
 * LastModified: Oct 10, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#include <hprose/io/HproseWriter.h>

#include <gtest/gtest.h>

TEST(HproseWriter, WriteNull) {
    std::ostringstream stream;
    hprose::HproseWriter writer(stream);
    writer.WriteNull();
    EXPECT_EQ(stream.str(), "n");
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
