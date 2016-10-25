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
 * hprose/io/decoders/IntDecoder.h                        *
 *                                                        *
 * hprose int decoder for cpp.                            *
 *                                                        *
 * LastModified: Oct 25, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <cstdint>

namespace hprose {
namespace io {

class Reader;

namespace decoders {

int64_t IntDecode(Reader &reader, char tag);

}
}
} // hprose::io::decoders
