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
 * LastModified: Oct 25, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <hprose/io/Tags.h>
#include <hprose/io/decoders/BoolDecoder.h>
#include <hprose/io/decoders/IntDecoder.h>

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

class Reader {
public:
    Reader(std::istream &stream, bool simple = false)
        : stream(stream), refer(simple ? nullptr : new internal::ReaderRefer()) {
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

    int64_t readInt64(char tag);

    inline int readLength() {
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

    template<class T>
    typename std::enable_if<
        std::is_same<T, float>::value,
        T
    >::type
    readFloat() {
        return std::stof(readUntil(tags::TagSemicolon));
    }

    template<class T>
    typename std::enable_if<
        std::is_same<T, double>::value,
        T
    >::type
    readFloat() {
        return std::stod(readUntil(tags::TagSemicolon));
    }

    template<class T>
    typename std::enable_if<
        std::is_same<T, long double>::value,
        T
    >::type
    readFloat() {
        return std::stold(readUntil(tags::TagSemicolon));
    }

    std::string readUTF8String(int length);

    inline std::string readString() {
        std::string s = readUTF8String(readLength());
        stream.get();
        return s;
    }

    std::string readStringWithoutTag() {
        return readString();
    }

    std::string readUntil(char tag) {
        std::string s;
        std::getline(stream, s, tag);
        return s;
    }

    std::istream &stream;

private:
    std::unique_ptr<internal::ReaderRefer> refer;
};

}
} // hprose::io

#include <hprose/io/Reader-inl.h>
