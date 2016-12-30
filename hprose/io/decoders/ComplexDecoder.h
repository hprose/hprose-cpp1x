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
 * hprose/io/decoders/ComplexDecoder.h                    *
 *                                                        *
 * hprose complex decoder for cpp.                        *
 *                                                        *
 * LastModified: Dec 30, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <complex>

namespace hprose {
namespace io {

class Reader;

namespace decoders {

template<class T>
void ComplexDecode(std::complex<T> &v, Reader &reader, char tag);

}
}
} // hprose::io::decoders
