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
 * hprose/io/ByteReader.h                                 *
 *                                                        *
 * hprose byte reader for cpp.                            *
 *                                                        *
 * LastModified: Dec 15, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <hprose/io/Tags.h>
#include <hprose/util/Util.h>

#include <istream>
#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace hprose {
namespace io {

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

    inline int readInt() {
        return readArithmetic<int>(TagSemicolon);
    }

    inline int readLength() {
        return readArithmetic<int>(TagQuote);
    }

    template<class T>
    typename std::enable_if<
        std::is_floating_point<T>::value,
        T
    >::type
    readInfinity() {
        return stream.get() == TagPos ? std::numeric_limits<T>::infinity() : -std::numeric_limits<T>::infinity();
    }

    template<class T>
    typename std::enable_if<
        std::is_floating_point<T>::value,
        T
    >::type
    readFloat() {
        return util::StringToFloat<T>(readUntil(TagSemicolon));
    }

    std::string read(size_t count);

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

}
} // hprose::io
