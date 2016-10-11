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
 * hprose/io/Writer.cpp                                   *
 *                                                        *
 * hprose writer unit for cpp.                            *
 *                                                        *
 * LastModified: Oct 11, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#include <hprose/io/Writer.h>
#include <hprose/io/Tags.h>

namespace hprose { namespace io {

void Writer::WriteNull() {
    stream << tags::TagNull;
}

void Writer::WriteBool(bool b) {
    stream << (b ? tags::TagTrue : tags::TagFalse);
}

} } // namespace
