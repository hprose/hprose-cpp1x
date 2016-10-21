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
 * LastModified: Oct 21, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <hprose/io/Tags.h>
#include <hprose/io/decoders/BoolDecoder.h>

#include <istream>
#include <sstream>
#include <memory>
#include <numeric>
#include <limits>
#include <type_traits>

namespace hprose {
namespace io {

namespace internal {

class ReaderRefer {
};

} // hprose::io::internal

template<class T>
struct Decoder;

class Reader {
public:
    Reader(std::istream &stream, bool simple = false)
        : stream(stream), refer(simple ? nullptr : new internal::ReaderRefer()) {
    }

    template<class T>
    inline T unserialize() {
        T v;
        readValue(v);
        return std::move(v);
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
        return decoders::BoolDecode(*this, stream.get());
    }

    int64_t readInt64(char tag) {
        int64_t i = 0;
        auto b = stream.get();
        if (b == tag) {
            return i;
        }
        bool neg = false;
        switch (b) {
            case '-':
                neg = true;
            case '+':
                b = stream.get();
        }
        if (neg) {
            while (b != tag) {
                i = i * 10 - (b - '0');
                b = stream.get();
            }
        } else {
            while
                (b != tag) {
                i = i * 10 + (b - '0');
                b = stream.get();
            }
        }
        return i;
    }

    int readLength() {
        return static_cast<int>(readInt64(tags::TagQuote));
    }

    template<class T>
    typename std::enable_if<
        std::is_floating_point<T>::value,
        T
    >::type
    readInfinity() {
        return stream.get() == tags::TagPos ? std::numeric_limits<T>::infinity() : -std::numeric_limits<T>::infinity();
    }

    std::string readUTF8String(int length) {
        std::stringstream ss;
        if (length == 0) {
            return ss.str();
        }
        for (auto i = 0; i < length; i++) {
            int c = stream.get();
            switch (c >> 4) {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7: {
                    ss << static_cast<char>(c);
                    break;
                }
                case 12:
                case 13: {
                    ss << static_cast<char>(c);
                    ss << static_cast<char>(stream.get());
                    break;
                }
                case 14: {
                    ss << static_cast<char>(c);
                    ss << static_cast<char>(stream.get());
                    ss << static_cast<char>(stream.get());
                    break;
                }
                case 15: {
                    if ((c & 8) == 8) {
                        throw "bad utf8";
                    }
                    i++;
                }
                default:
                    throw "bad utf8";
            };
        }
        stream.get();
        return ss.str();
    }

    std::string readString() {
        std::string s = readUTF8String(readLength());
        return s;
    }

    std::string readStringWithoutTag() {
        return readString();
    }

    std::string readUntil(char tag) {
        std::string s;
        std::getline(stream, s, tag);
        return std::move(s);
    }

    std::istream &stream;

private:

    std::unique_ptr<internal::ReaderRefer> refer;
};

}
} // hprose::io

#include <hprose/io/Reader-inl.h>
