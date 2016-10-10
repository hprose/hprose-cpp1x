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
 * HproseWriter.h                                         *
 *                                                        *
 * hprose writer header for cpp.                          *
 *                                                        *
 * LastModified: Oct 10, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <ostream>

namespace hprose {

class HproseWriter {
public:

    HproseWriter(std::ostream &stream, bool simple = false)
        : stream(stream), simple(simple) {
    }

    void WriteNull();

    void WriteBool(const bool b);

private:

    std::ostream &stream;
    bool simple;
};

}
