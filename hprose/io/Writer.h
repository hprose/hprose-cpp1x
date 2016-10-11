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
 * LastModified: Oct 11, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <hprose/io/Types.h>

#include <ostream>

namespace hprose { namespace io {

class Writer {
public:

    Writer(std::ostream &stream, bool simple = false)
        : stream(stream), simple(simple) {
    }

    template<typename T>
    inline void serialize(const T &v) {
        writeValue(v);
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
        }
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

    std::ostream &stream;
    bool simple;
};

} } // hprose::io
