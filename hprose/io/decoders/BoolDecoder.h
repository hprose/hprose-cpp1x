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
 * hprose/io/decoders/BoolDecoder.h                       *
 *                                                        *
 * hprose bool decoder for cpp.                           *
 *                                                        *
 * LastModified: Oct 21, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <hprose/io/Tags.h>

namespace hprose {
namespace io {

class Reader;

namespace decoders {

bool BoolDecode(Reader &reader, char tag) {
    switch (tag) {
        case '0':
        case tags::TagNull:
        case tags::TagEmpty:
        case tags::TagFalse:
            return false;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case tags::TagTrue:
        case tags::TagNaN:
            return true;
        default:
            return false;
    }
}

}
}
} // hprose::io::decoders