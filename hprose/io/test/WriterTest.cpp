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

#define T(value, expected) { \
    std::ostringstream stream; \
    hprose::io::Writer writer(stream); \
    writer.serialize(value); \
    EXPECT_EQ(stream.str(), expected); \
}

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
    T(true, "t");
    T(false, "f");
}

TEST(Writer, SerializeDigit) {
    for (int i = 0; i <= 9; i++) {
        T(i, std::to_string(i));
    }
}

TEST(Writer, SerializeInteger) {
    std::random_device rd;

    std::uniform_int_distribution<int32_t> dis1(10); 
    for (int i = 0; i < 100; i++) {
        int32_t x = dis1(rd);
        T(x, "i" + std::to_string(x) + ";");
    }

    std::uniform_int_distribution<int64_t> dis2(static_cast<int64_t>(std::numeric_limits<int32_t>::max()) + 1);
    for (int i = 0; i < 100; i++) {
        int64_t x = dis2(rd);
        T(x, "l" + std::to_string(x) + ";");
    }
}

TEST(Writer, SerializeFloat) {
    T(std::numeric_limits<float>::quiet_NaN(), "N");
    T(std::numeric_limits<float>::infinity(), "I+");
    T(-std::numeric_limits<float>::infinity(), "I-");
    T(3.14159f, "d3.14159;");

    T(std::numeric_limits<double>::quiet_NaN(), "N");
    T(std::numeric_limits<double>::infinity(), "I+");
    T(-std::numeric_limits<double>::infinity(), "I-");
    T(3.14159265358979, "d3.14159265358979;");

    T(std::numeric_limits<long double>::quiet_NaN(), "N");
    T(std::numeric_limits<long double>::infinity(), "I+");
    T(-std::numeric_limits<long double>::infinity(), "I-");
    T(3.14159265358979324l, "d3.14159265358979324;");
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
