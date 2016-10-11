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

#include <ostream>

namespace hprose { namespace io {

class Writer {
public:

    Writer(std::ostream &stream, bool simple = false)
        : stream(stream), simple(simple) {
    }

    void WriteNull();

    void WriteBool(bool b);

    template<typename ValueType>
    void WriteInteger(const ValueType &i);

private:

    std::ostream &stream;
    bool simple;
};

} } // namespace
