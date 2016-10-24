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
 * LastModified: Oct 22, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

namespace hprose {
namespace io {

class Reader;

namespace decoders {

bool BoolDecode(Reader &reader, char tag);

}
}
} // hprose::io::decoders
