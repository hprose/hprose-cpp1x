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
        return readValue<T>();
    }

    template<class T>
    inline T readValue() {
        return Decoder<T>().decode(*this);
    }

    inline bool readBool() {
        return decoders::BoolDecode(*this, stream.get());
    }

    std::istream &stream;

private:
    std::unique_ptr<internal::ReaderRefer> refer;
};

}
} // hprose::io

#include <hprose/io/Reader-inl.h>
