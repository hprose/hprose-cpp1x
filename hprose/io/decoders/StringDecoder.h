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
 * hprose/io/decoders/StringDecoder.h                     *
 *                                                        *
 * hprose string decoder for cpp.                         *
 *                                                        *
 * LastModified: Nov 9, 2016                              *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <string>

namespace hprose {
namespace io {

class Reader;

namespace decoders {

std::string StringDecode(Reader &reader, char tag);

}
}
} // hprose::io::decoders
