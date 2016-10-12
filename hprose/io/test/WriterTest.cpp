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
 * WriterTest.cpp                                         *
 *                                                        *
 * hprose writer test for cpp.                            *
 *                                                        *
 * LastModified: Oct 12, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#include <hprose/io/Writer.h>

#include <gtest/gtest.h>

#include <random>

TEST(Writer, WriteNull) {
    std::ostringstream stream;
    hprose::io::Writer writer(stream);
    writer.writeNull();
    EXPECT_EQ(stream.str(), "n");
}

TEST(Writer, WriteBool) {
    std::ostringstream stream;
    hprose::io::Writer writer(stream);
    writer.writeBool(true);
    EXPECT_EQ(stream.str(), "t");
    stream.str("");
    writer.writeBool(false);
    EXPECT_EQ(stream.str(), "f");
}

TEST(Writer, SerializeBool) {
    std::ostringstream stream;
    hprose::io::Writer writer(stream);
    writer.serialize(true);
    EXPECT_EQ(stream.str(), "t");
    stream.str("");
    writer.serialize(false);
    EXPECT_EQ(stream.str(), "f");
}

TEST(Writer, SerializeDigit) {
    std::ostringstream stream;
    hprose::io::Writer writer(stream);
    for (int i = 0; i <= 9; i++) {
        stream.str("");
        writer.serialize(i);
        EXPECT_EQ(stream.str(), std::to_string(i));
    }
}

TEST(Writer, SerializeInteger) {
    std::ostringstream stream;
    hprose::io::Writer writer(stream);
    std::random_device rd;
    std::uniform_int_distribution<int32_t> dis1(10); 
    for (int i = 0; i <= 100; i++) {
        stream.str("");
        int32_t x = dis1(rd);
        writer.serialize(x);
        EXPECT_EQ(stream.str(), "i" + std::to_string(x) + ";");
    }
    std::uniform_int_distribution<int64_t> dis2(static_cast<int64_t>(std::numeric_limits<int32_t>::max()) + 1);
    for (int i = 0; i <= 100; i++) {
        stream.str("");
        int64_t x = dis2(rd);
        writer.serialize(x);
        EXPECT_EQ(stream.str(), "l" + std::to_string(x) + ";");
    }
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
