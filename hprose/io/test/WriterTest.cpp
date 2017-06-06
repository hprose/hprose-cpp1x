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
 * LastModified: Nov 18, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#include <hprose/io/Writer.h>
#include <hprose/io/Reader.h>

#include <gtest/gtest.h>

#include <random>

#define T(value, expected) { \
    std::ostringstream stream; \
    hprose::io::Writer writer(stream, true); \
    writer.serialize(value); \
    EXPECT_EQ(stream.str(), expected); \
}

TEST(Writer, SerializeNull) {
    T(nullptr, "n");
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

TEST(Writer, SerializeInt) {
    std::random_device rd;

    std::uniform_int_distribution<int32_t> dis1(10); 
    for (int i = 0; i < 100; i++) {
        auto x = dis1(rd);
        T(x, "i" + std::to_string(x) + ";");
    }

    std::uniform_int_distribution<int64_t> dis2(static_cast<int64_t>(std::numeric_limits<int32_t>::max()) + 1);
    for (int i = 0; i < 100; i++) {
        auto x = dis2(rd);
        T(x, "l" + std::to_string(x) + ";");
    }
}

TEST(Writer, SerializeInt8) {
    for (auto i = 0; i <= 9; i++) {
        T(static_cast<int8_t>(i), std::to_string(i));
    }

    for (auto i = 10; i < 128; i++) {
        T(static_cast<int8_t>(i), "i" + std::to_string(i) + ";");
    }

    for (auto i = -128; i < 0; i++) {
        T(static_cast<int8_t>(i), "i" + std::to_string(i) + ";");
    }
}

TEST(Writer, SerializeInt16) {
    auto i = std::numeric_limits<int16_t>::max();
    T(i, "i" + std::to_string(i) + ";");
}

TEST(Writer, SerializeInt32) {
    auto i = std::numeric_limits<int32_t>::max();
    T(i, "i" + std::to_string(i) + ";");
}

TEST(Writer, SerializeInt64) {
    int64_t i1 = std::numeric_limits<int32_t>::max();
    T(i1, "i" + std::to_string(i1) + ";");
    int64_t i2 = std::numeric_limits<int64_t>::max();
    T(i2, "l" + std::to_string(i2) + ";");
}

TEST(Writer, SerializeUint) {
    std::random_device rd;

    std::uniform_int_distribution<int32_t> dis1(10); 
    for (auto i = 0; i < 100; i++) {
        uint32_t x = dis1(rd);
        T(x, "i" + std::to_string(x) + ";");
    }

    std::uniform_int_distribution<int64_t> dis2(static_cast<int64_t>(std::numeric_limits<int32_t>::max()) + 1);
    for (auto i = 0; i < 100; i++) {
        uint64_t x = dis2(rd);
        T(x, "l" + std::to_string(x) + ";");
    }
}

TEST(Writer, SerializeUint8) {
    for (auto u = 0; u <= 9; u++) {
        T(static_cast<uint8_t>(u), std::to_string(u));
    }

    for (auto u = 10; u < 256; u++) {
        T(static_cast<uint8_t>(u), "i" + std::to_string(u) + ";");
    }
}

TEST(Writer, SerializeUint16) {
    auto u = std::numeric_limits<uint16_t>::max();
    T(u, "i" + std::to_string(u) + ";");
}

TEST(Writer, SerializeUint32) {
    auto u = std::numeric_limits<uint32_t>::max();
    T(u, "l" + std::to_string(u) + ";");
}

TEST(Writer, SerializeUint64) {
    uint64_t u1 = std::numeric_limits<uint32_t>::max();
    T(u1, "l" + std::to_string(u1) + ";");
    uint64_t u2 = std::numeric_limits<uint64_t>::max();
    T(u2, "l" + std::to_string(u2) + ";");
}

enum Color { red, green, blue };

TEST(Writer, SerializeEnum) {
    T(red, "0");
    T(green, "1");
    T(blue, "2");
}

TEST(Writer, SerializeFloat) {
    T(std::numeric_limits<float>::quiet_NaN(), "N");
    T(std::numeric_limits<float>::infinity(), "I+");
    T(-std::numeric_limits<float>::infinity(), "I-");
    T(3.14159265358979323846f, "d3.14159;");

    T(std::numeric_limits<double>::quiet_NaN(), "N");
    T(std::numeric_limits<double>::infinity(), "I+");
    T(-std::numeric_limits<double>::infinity(), "I-");
    T(3.14159265358979323846, "d3.14159265358979;");

    T(std::numeric_limits<long double>::quiet_NaN(), "N");
    T(std::numeric_limits<long double>::infinity(), "I+");
    T(-std::numeric_limits<long double>::infinity(), "I-");
    //T(3.14159265358979323846l, "d3.14159265358979324;");
}

TEST(Writer, SerializeComplex) {
    T(std::complex<float>(100.f), "d100;");
    T(std::complex<float>(0, 100.f), "a2{d0;d100;}");

    T(std::complex<double>(100.f), "d100;");
    T(std::complex<double>(0, 100.f), "a2{d0;d100;}");

    T(std::complex<long double>(100.f), "d100;");
    T(std::complex<long double>(0, 100.f), "a2{d0;d100;}");
}

TEST(Writer, SerializeRatio) {
    T(std::ratio<123>(), "i123;");
    T((std::ratio<123, 2>()), R"(s5"123/2")");
}

TEST(Writer, SerializeString) {
    T(u8"", "e");
    T(u8"π", u8"uπ");
    T(u8"你", u8"u你");
    T(u8"你好", u8R"(s2"你好")");
    T(u8"你好啊,hello!", u8R"(s10"你好啊,hello!")");
    T(u8"🇨🇳", u8R"(s4"🇨🇳")");
    T("\x80\x81\x82", "b3\"\x80\x81\x82\"");

#ifdef HPROSE_HAS_CODECVT
    T(L"", "e");
    T(L"π", u8"uπ");
    T(L"你", u8"u你");
    T(L"你好", u8R"(s2"你好")");
    T(L"你好啊,hello!", u8R"(s10"你好啊,hello!")");
    T(L"🇨🇳", u8R"(s4"🇨🇳")");

    T(u"", "e");
    T(u"π", u8"uπ");
    T(u"你", u8"u你");
    T(u"你好", u8R"(s2"你好")");
    T(u"你好啊,hello!", u8R"(s10"你好啊,hello!")");
    T(u"🇨🇳", u8R"(s4"🇨🇳")");

    T(U"", "e");
    T(U"π", u8"uπ");
    T(U"你", u8"u你");
    T(U"你好", u8R"(s2"你好")");
    T(U"你好啊,hello!", u8R"(s10"你好啊,hello!")");
    T(U"🇨🇳", u8R"(s4"🇨🇳")");
#endif // HPROSE_HAS_CODECVT

    T(std::string(u8""), "e");
    T(std::string(u8"π"), u8"uπ");
    T(std::string(u8"你"), u8"u你");
    T(std::string(u8"你好"), u8R"(s2"你好")");
    T(std::string(u8"你好啊,hello!"), u8R"(s10"你好啊,hello!")");
    T(std::string(u8"🇨🇳"), u8R"(s4"🇨🇳")");
    T(std::string("\x80\x81\x82"), "b3\"\x80\x81\x82\"");

#ifdef HPROSE_HAS_CODECVT
    T(std::wstring(L""), "e");
    T(std::wstring(L"π"), u8"uπ");
    T(std::wstring(L"你"), u8"u你");
    T(std::wstring(L"你好"), u8R"(s2"你好")");
    T(std::wstring(L"你好啊,hello!"), u8R"(s10"你好啊,hello!")");
    T(std::wstring(L"🇨🇳"), u8R"(s4"🇨🇳")");

    T(std::u16string(u""), "e");
    T(std::u16string(u"π"), u8"uπ");
    T(std::u16string(u"你"), u8"u你");
    T(std::u16string(u"你好"), u8R"(s2"你好")");
    T(std::u16string(u"你好啊,hello!"), u8R"(s10"你好啊,hello!")");
    T(std::u16string(u"🇨🇳"), u8R"(s4"🇨🇳")");

    T(std::u32string(U""), "e");
    T(std::u32string(U"π"), u8"uπ");
    T(std::u32string(U"你"), u8"u你");
    T(std::u32string(U"你好"), u8R"(s2"你好")");
    T(std::u32string(U"你好啊,hello!"), u8R"(s10"你好啊,hello!")");
    T(std::u32string(U"🇨🇳"), u8R"(s4"🇨🇳")");
#endif // HPROSE_HAS_CODECVT
}

std::tm makeTm(int year, int month, int day, int hour = 0, int min = 0, int sec = 0, int gmtoff = 0) {
    std::tm tm;
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min = min;
    tm.tm_sec = sec;
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#else
    tm.tm_gmtoff = gmtoff;
#endif
    return tm;
}

TEST(Writer, SerializeTime) {
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    T(makeTm(1980, 12, 1), "D19801201;");
    T(makeTm(1970, 1, 1, 12, 34, 56), "T123456;");
    T(makeTm(1980, 12, 1, 12, 34, 56), "D19801201T123456;");
#else
    T(makeTm(1980, 12, 1), "D19801201Z");
    T(makeTm(1970, 1, 1, 12, 34, 56), "T123456Z");
    T(makeTm(1980, 12, 1, 12, 34, 56), "D19801201T123456Z");
    T(makeTm(1980, 12, 1, 0, 0, 0, 28800), "D19801201;");
    T(makeTm(1970, 1, 1, 12, 34, 56, 28800), "T123456;");
    T(makeTm(1980, 12, 1, 12, 34, 56, 28800), "D19801201T123456;");
#endif
}

TEST(Writer, SerializePointer) {
    int *p = nullptr;
    T(p, "n");

    int i = 123;
    p = &i;
    T(p, "i123;");

    std::unique_ptr<int> up(new int(123));
    T(up, "i123;");

    std::weak_ptr<int> wp;

    {
        std::shared_ptr<int> sp1(new int(123));
        std::shared_ptr<int> sp2(sp1);
        T(sp1, "i123;");
        T(sp2, "i123;");

        wp = sp1;
        T(wp, "i123;");
    }

    std::ostringstream stream;
    hprose::io::Writer writer(stream, true);
    EXPECT_THROW(writer.serialize(wp), std::runtime_error);
}

TEST(Writer, SerializeList) {
    int a1[] = {1, 2, 3};
    uint8_t a2[] = {'h', 'e', 'l', 'l', 'o'};
    T(a1, "a3{123}");
    T(a2, R"(b5"hello")");

#ifdef HPROSE_HAS_ARRAY_INITIALIZER_LIST
    T((std::array<int, 3>({{1, 2, 3}})), "a3{123}");
    T((std::array<double, 3>({{1, 2, 3}})), "a3{d1;d2;d3;}");
    T((std::array<bool, 3>({{true, false, true}})), "a3{tft}");
    T((std::array<int, 0>()), "a{}");
    T((std::array<bool, 0>()), "a{}");
#else // HPROSE_HAS_ARRAY_INITIALIZER_LIST
    {
        std::array<int, 3> var;
        var[0] = 1;
        var[1] = 2;
        var[2] = 3;
        T(var, "a3{123}");
    }
    {
        std::array<double, 3> var;
        var[0] = 1;
        var[1] = 2;
        var[2] = 3;
        T(var, "a3{d1;d2;d3;}");
    }
    {
        std::array<bool, 3> var;
        var[0] = true;
        var[1] = false;
        var[2] = true;
        T(var, "a3{tft}");
    }
    {
        std::array<int, 0> var;
        T(var, "a{}");
    }
    {
        std::array<bool, 0> var;
        T(var, "a{}");
    }
#endif // HPROSE_HAS_ARRAY_INITIALIZER_LIST

    T(std::vector<uint8_t>({'h', 'e', 'l', 'l', 'o'}), R"(b5"hello")");
    T(std::vector<uint8_t>(), R"(b"")");

    T(std::vector<int>({1, 2, 3}), "a3{123}");
    T(std::vector<double>({1, 2, 3}), "a3{d1;d2;d3;}");
    T(std::vector<bool>({true, false, true}), "a3{tft}");
    T(std::vector<int>(), "a{}");
    T(std::vector<bool>(), "a{}");

    T(std::deque<int>({1, 2, 3}), "a3{123}");
    T(std::forward_list<int>({1, 2, 3}), "a3{123}");
    T(std::list<int>({1, 2, 3}), "a3{123}");
    T(std::set<int>({1, 2, 3}), "a3{123}");
    T(std::multiset<int>({1, 2, 3}), "a3{123}");
}

TEST(Writer, SerializeBitset) {
    T(std::bitset<0>(), "a{}");
    T(std::bitset<8>(), "a8{ffffffff}");   // [0,0,0,0,0,0,0,0]
    T(std::bitset<8>(42), "a8{ftftftff}"); // [0,0,1,0,1,0,1,0]
}

TEST(Writer, SerializeTuple) {
    T(std::make_tuple(), "a{}");
    T(std::make_tuple(1, 3.14, true), "a3{1d3.14;t}");
}

TEST(Writer, SerializeMap) {
    std::map<int, float> map;
    T(map, "m{}");
    map[4] = 4.13;
    map[9] = 9.24;
    map[1] = 1.09;
    T(map, "m3{1d1.09;4d4.13;9d9.24;}");
}

struct TestStruct {
    int ID;
};

struct TestStruct1 : TestStruct {
    std::string Name; 
    int *Age;  
};

struct TestStruct2 : TestStruct1 {
    bool OOXX;
    TestStruct2 *pStruct;
    TestStruct Test;
    std::tm birthday;
};

HPROSE_REG_CLASS(TestStruct, "Test", {
    HPROSE_REG_FIELD(ID, "id");
})

HPROSE_REG_CLASS(TestStruct1, "Test1", {
    HPROSE_REG_FIELD(ID, "id");
    HPROSE_REG_FIELD(Name, "name");
    HPROSE_REG_FIELD(Age, "age");
})

HPROSE_REG_CLASS(TestStruct2, "Test2", {
    HPROSE_REG_FIELD(OOXX, "ooxx");
    HPROSE_REG_FIELD(pStruct, "testStruct2");
    HPROSE_REG_FIELD(ID, "id");
    HPROSE_REG_FIELD(Name, "name");
    HPROSE_REG_FIELD(Age, "age");
    HPROSE_REG_FIELD(Test, "test");
})

TEST(Writer, SerializeStruct) {
    TestStruct2 st;
    st.pStruct = &st;
    st.ID = 100;
    st.Name = "Tom";
    auto age = 18;
    st.Age = &age;
    st.OOXX = false;
    st.Test.ID = 200;
     std::ostringstream stream;
    hprose::io::Writer writer(stream, false);
    writer.serialize(st);
    EXPECT_EQ(stream.str(), R"(c5"Test2"6{s4"ooxx"s11"testStruct2"s2"id"s4"name"s3"age"s4"test"}o0{fr6;i100;s3"Tom"i18;c4"Test"1{s2"id"}o1{i200;}})");
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
