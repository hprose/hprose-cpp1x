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
 * hprose/io/decoders/ObjectDecoder.h                     *
 *                                                        *
 * hprose object decoder for cpp.                         *
 *                                                        *
 * LastModified: Dec 18, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

namespace hprose {
namespace io {

class Reader;

namespace decoders {

template<class T>
void ObjectDecode(T &v, Reader &reader, char tag);

}
}
} // hprose::io::decoders
