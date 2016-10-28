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
 * hprose/io/Reader.h                                     *
 *                                                        *
 * hprose reader header for cpp.                          *
 *                                                        *
 * LastModified: Oct 27, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <hprose/io/Tags.h>
#include <hprose/io/decoders/BoolDecoder.h>
#include <hprose/io/decoders/IntDecoder.h>
#include <hprose/io/decoders/FloatDecoder.h>
#include <hprose/util/Util.h>

#include <istream>
#include <sstream>
#include <memory>
#include <numeric>
#include <limits>
#include <type_traits>

namespace hprose {
namespace io {

namespace internal {

std::string TagToString(char tag);

class ReaderRefer {
};

} // hprose::io::internal

struct UnexpectedTag : std::runtime_error {
    explicit UnexpectedTag(char tag)
        : std::runtime_error(tag == -1 ?
                             "no byte found in stream" :
                             std::string("unexpected serialize tag '") + tag + "' in stream") {
    }
};

template<class T>
struct CastError : std::runtime_error {
    explicit CastError(char tag)
        : std::runtime_error(internal::TagToString(tag) + " can't change to " + typeid(T).name()) {
    }
};

template<class T>
struct Decoder;

class ByteReader {
public:
    ByteReader(std::istream &stream)
        : stream(stream) {
    }

    int read2Digit() {
        auto n = stream.get() - '0';
        return n * 10 + (stream.get() - '0');
    }

    int read4Digit() {
        auto n = stream.get() - '0';
        n = n * 10 + (stream.get() - '0');
        n = n * 10 + (stream.get() - '0');
        return n * 10 + (stream.get() - '0');
    }

    template<class T>
    typename std::enable_if<
        std::is_arithmetic<T>::value,
        T
    >::type
    readArithmetic(char tag) {
        auto b = stream.get();
        if (b == tag) {
            return 0;
        }
        T i = 0;
        auto neg = false;
        switch (b) {
            case '-':
                neg = true;
            case '+':
                b = stream.get();
            default:
                break;
        }
        if (neg) {
            while (b != tag) {
                i = i * 10 - (b - '0');
                b = stream.get();
            }
        } else {
            while (b != tag) {
                i = i * 10 + (b - '0');
                b = stream.get();
            }
        }
        return i;
    }

    inline int readLength() {
        return readArithmetic<int>(tags::TagQuote);
    }

    template<class T>
    typename std::enable_if<
        std::is_floating_point<T>::value,
        T
    >::type
    readInfinity() {
        return stream.get() == tags::TagPos ? std::numeric_limits<T>::infinity() : -std::numeric_limits<T>::infinity();
    }

    template<class T>
    typename std::enable_if<
        std::is_floating_point<T>::value,
        T
    >::type
    readFloat() {
        return util::StringToFloat<T>(readUntil(tags::TagSemicolon));
    }

    std::string readUntil(char tag) {
        std::string s;
        std::getline(stream, s, tag);
        return s;
    }

    std::string readUTF8String(int length);

    inline std::string readString() {
        std::string s = readUTF8String(readLength());
        stream.get();
        return s;
    }

    std::istream &stream;
};

class Reader : public ByteReader {
public:
    Reader(std::istream &stream, bool simple = false)
        : ByteReader(stream), refer(simple ? nullptr : new internal::ReaderRefer()) {
    }

    template<class T>
    inline T unserialize() {
        T v;
        readValue(v);
        return v;
    }

    template<class T>
    inline Reader &unserialize(T &v) {
        readValue(v);
        return *this;
    }

    template<class T>
    inline void readValue(T &v) {
        return decode(v, *this);
    }

    inline bool readBool() {
        return decoders::BoolDecode(*this, static_cast<char>(stream.get()));
    }

    template<class T>
    inline typename std::enable_if<
        std::is_integral<T>::value,
        T
    >::type
    readInteger() {
        return decoders::IntDecode(*this, static_cast<char>(stream.get()));
    }

    template<class T>
    typename std::enable_if<
        std::is_floating_point<T>::value,
        T
    >::type
    readFloat() {
        return decoders::FloatDecode<T>(*this, static_cast<char>(stream.get()));
    }

    std::string readStringWithoutTag() {
        return readString();
    }

    std::tm readDateTimeWithoutTag() {
        std::tm tm;
        tm.tm_year = read4Digit() - 1900;
        tm.tm_mon = read2Digit() - 1;
        tm.tm_mday = read2Digit();
        auto tag = stream.get();
        if (tag == tags::TagTime) {
            tm.tm_hour = read2Digit();
            tm.tm_min = read2Digit();
            tm.tm_sec = read2Digit();
            tag = stream.get();
            if (tag == tags::TagPoint) {
            }
        } else {
            tm.tm_hour = 0;
            tm.tm_min = 0;
            tm.tm_sec = 0;
        }
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)

#else
        if (tag == tags::TagUTC) {
            tm.tm_gmtoff = 0;
        } else {
            std::time_t t = time(0);
            std::tm temp;
            localtime_r(&t, &temp);
            tm.tm_isdst = temp.tm_isdst;
            tm.tm_gmtoff = temp.tm_gmtoff;
            tm.tm_zone = temp.tm_zone;
            mktime(&tm);
        }
#endif
        return tm;
    }

private:
    std::unique_ptr<internal::ReaderRefer> refer;
};

}
} // hprose::io

#include <hprose/io/decoders/FloatDecoder-inl.h>
#include <hprose/io/Reader-inl.h>
