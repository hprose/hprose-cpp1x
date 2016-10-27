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
 * hprose/io/decoders/FloatDecoder.h                      *
 *                                                        *
 * hprose float decoder for cpp.                          *
 *                                                        *
 * LastModified: Oct 27, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <type_traits>

namespace hprose {
namespace io {

class Reader;

namespace decoders {

template<class T>
typename std::enable_if<
    std::is_floating_point<T>::value,
    T
>::type
FloatDecode(Reader &reader, char tag);

}
}
} // hprose::io::decoders
