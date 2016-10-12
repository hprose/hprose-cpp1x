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
 * LastModified: Oct 12, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#include <hprose/io/Writer.h>

namespace hprose { namespace io {

void Writer::writeNull() {
    stream << tags::TagNull;
}

void Writer::writeBool(bool b) {
    stream << (b ? tags::TagTrue : tags::TagFalse);
}

} } // hprose::io
