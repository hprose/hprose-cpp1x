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
 * hprose/io/Writer.h                                     *
 *                                                        *
 * hprose writer header for cpp.                          *
 *                                                        *
 * LastModified: Oct 12, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <hprose/io/Tags.h>
#include <hprose/io/Types.h>

#include <ostream>
#include <numeric>

namespace hprose { namespace io {

class Writer {
public:

    Writer(std::ostream &stream, bool simple = false)
        : stream(stream), simple(simple) {
    }

    template<typename T>
    inline Writer& serialize(const T &v) {
        writeValue(v);
        return *this;
    }

    template<typename T>
    inline void writeValue(const T &v) {
        writeValue(v, NonCVType<T>());
    }

    void writeNull();

    void writeBool(bool b);

    template<typename T>
    void writeInteger(T i) {
        if (i >= 0 && i <= 9) {
            stream << static_cast<char>('0' + i);
            return;
        }
        if (i >= std::numeric_limits<int32_t>::min() && i <= std::numeric_limits<int32_t>::max()) {
            stream << tags::TagInteger;
        } else {
            stream << tags::TagLong;
        }
        stream << i << tags::TagSemicolon;
    }

    template<typename T>
    void writeFloat(T f);

private:

    template<typename T>
    inline void writeValue(const T &, UnknownType) {
    }

    template<typename T>
    inline void writeValue(const T &b, BoolType) {
        writeBool(b);
    }

    template<typename T>
    inline void writeValue(const T &i, IntegerType) {
        writeInteger(i);
    }

    std::ostream &stream;
    bool simple;
};

} } // hprose::io
