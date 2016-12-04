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
 * ReaderTest.cpp                                         *
 *                                                        *
 * hprose reader test for cpp.                            *
 *                                                        *
 * LastModified: Dec 4, 2016                              *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#include <hprose/io/Writer.h>
#include <hprose/io/Reader.h>

#include <gtest/gtest.h>

using hprose::io::Writer;
using hprose::io::Reader;

#define T(Type, value, expected) { \
    std::stringstream stream; \
    Writer writer(stream, true); \
    writer.serialize(value); \
    Reader reader(stream, true); \
    EXPECT_EQ(reader.unserialize<Type>(), expected); \
}

#define T_R(Type, value, expected) { \
    std::stringstream stream; \
    Writer writer(stream, false); \
    writer.serialize(value).serialize(value); \
    Reader reader(stream, false); \
    EXPECT_EQ(reader.unserialize<Type>(), expected); \
    EXPECT_EQ(reader.unserialize<Type>(), expected); \
}

TEST(Reader, UnserializeBool) {
    std::string trueValue("true");
    T(bool, true, true);
    T(bool, false, false);
    T(bool, nullptr, false);
    T(bool, "", false);
    T(bool, 0, false);
    T(bool, 1, true);
    T(bool, 9, true);
    T(bool, 100, true);
    T(bool, 100000000000000, true);
    T(bool, 0.0, false);
    T(bool, "t", true);
    T(bool, "f", false);
    T(bool, "false", false);
    T_R(bool, trueValue, true);
}

TEST(Reader, UnserializeInt) {
    std::string intValue("1234567");
    T(int64_t, true, 1);
    T(int64_t, false, 0);
    T(int64_t, nullptr, 0);
    T(int64_t, "", 0);
    T(int64_t, 0, 0);
    T(int64_t, 1, 1);
    T(int64_t, 9, 9);
    T(int64_t, 100, 100);
    T(int64_t, -100, -100);
    T(int64_t, 99.9, 99);
    T(int64_t, std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::min());
    T(int64_t, std::numeric_limits<int64_t>::max(), std::numeric_limits<int64_t>::max());
    T(int64_t, std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::min());
    T(int64_t, std::numeric_limits<uint64_t>::max(), std::numeric_limits<uint64_t>::max());
    T(int64_t, 0.0, 0);
    T(int64_t, "1", 1);
    T(int64_t, "9", 9);
    T_R(int64_t, intValue, 1234567);
}

TEST(Reader, UnserializeFloat) {
    std::string floatValue("3.14159");
    T(float, true, 1.f);
    T(float, false, 0.f);
    T(float, nullptr, 0.f);
    T(float, "", 0.f);
    T(float, 0, 0.f);
    T(float, 1, 1.f);
    T(float, 9, 9.f);
    T(float, 100, 100.f);
    // T(float, std::numeric_limits<int64_t>::max(), static_cast<float>(std::numeric_limits<int64_t>::max()));
    // T(float, std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    T(float, 0.0, 0.f);
    T(float, "1", 1.f);
    T(float, "9", 9.f);
    T_R(float, floatValue, 3.14159f);
}

TEST(Reader, UnserializeDouble) {
    std::string doubleValue("3.14159");
    T(double, true, 1.0);
    T(double, false, 0.0);
    T(double, nullptr, 0.0);
    T(double, "", 0.0);
    T(double, 0, 0.0);
    T(double, 1, 1.0);
    T(double, 9, 9.0);
    T(double, 100, 100.0);
    // T(double, std::numeric_limits<float>::max(), static_cast<double>(std::numeric_limits<float>::max()));
    // T(double, std::numeric_limits<double>::max(), std::numeric_limits<double>::max());
    T(double, 0.0, 0.0);
    T(double, "1", 1.0);
    T(double, "9", 9.0);
    T_R(double, doubleValue, 3.14159);
}

TEST(Reader, UnserializeArray) {
    int a[] = {1, 2, 3, 4, 5};
    uint8_t b[] = {'h', 'e', 'l', 'l', 'o'};
    std::stringstream stream;
    Writer writer(stream, false);
    writer
        .serialize(a)
        .serialize(b)
        .serialize(a)
        .serialize(b)
        .serialize(nullptr)
        .serialize("");
    Reader reader(stream, false);

    int a1[5];
    reader.unserialize(a1);
    EXPECT_EQ(a1[0], 1);
    EXPECT_EQ(a1[1], 2);
    EXPECT_EQ(a1[2], 3);
    EXPECT_EQ(a1[3], 4);
    EXPECT_EQ(a1[4], 5);

    uint8_t b1[5];
    reader.unserialize(b1);
    EXPECT_EQ(b1[0], 'h');
    EXPECT_EQ(b1[1], 'e');
    EXPECT_EQ(b1[2], 'l');
    EXPECT_EQ(b1[3], 'l');
    EXPECT_EQ(b1[4], 'o');

    std::tuple<int, int, int, int, int> s;
    reader.unserialize(s);    
    EXPECT_EQ(std::get<0>(s), 1);
    EXPECT_EQ(std::get<1>(s), 2);
    EXPECT_EQ(std::get<2>(s), 3);
    EXPECT_EQ(std::get<3>(s), 4);
    EXPECT_EQ(std::get<4>(s), 5);
}

TEST(Reader, UnserializeList) {
    std::vector<int> a{1, 2, 3, 4, 5};
    std::vector<uint8_t> b{'h', 'e', 'l', 'l', 'o'};
    std::stringstream stream;
    Writer writer(stream, false);
    writer
        .serialize(a)
        .serialize(b)
        .serialize(a)
        .serialize(b);
    Reader reader(stream, false);

    std::vector<int> v1;
    reader.unserialize(v1);
    EXPECT_EQ(v1[0], 1);
    EXPECT_EQ(v1[1], 2);
    EXPECT_EQ(v1[2], 3);
    EXPECT_EQ(v1[3], 4);
    EXPECT_EQ(v1[4], 5);

    std::vector<uint8_t> v2;
    reader.unserialize(v2);
    EXPECT_EQ(v2[0], 'h');
    EXPECT_EQ(v2[1], 'e');
    EXPECT_EQ(v2[2], 'l');
    EXPECT_EQ(v2[3], 'l');
    EXPECT_EQ(v2[4], 'o');

    std::vector<int> v3;
    reader.unserialize(v3);
    EXPECT_EQ(v3[0], 1);
    EXPECT_EQ(v3[1], 2);
    EXPECT_EQ(v3[2], 3);
    EXPECT_EQ(v3[3], 4);
    EXPECT_EQ(v3[4], 5);

    std::vector<uint8_t> v4;
    reader.unserialize(v4);
    EXPECT_EQ(v4[0], 'h');
    EXPECT_EQ(v4[1], 'e');
    EXPECT_EQ(v4[2], 'l');
    EXPECT_EQ(v4[3], 'l');
    EXPECT_EQ(v4[4], 'o');
}

TEST(Reader, UnserializeMap) {
    std::unordered_map<std::string, std::string> map {
        {"name", "Tom"},
        {"国家", "🇨🇳"}
    };
    std::stringstream stream;
    Writer writer(stream, false); 
    writer.serialize(map);
    Reader reader(stream, false);
    std::unordered_map<std::string, std::string> map1;
    reader.unserialize(map1);
    EXPECT_EQ(map, map1);
}
