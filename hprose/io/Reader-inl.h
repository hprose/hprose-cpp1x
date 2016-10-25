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
 * hprose/io/Reader-inl.h                                 *
 *                                                        *
 * hprose decode funtions for cpp.                        *
 *                                                        *
 * LastModified: Oct 25, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

namespace hprose {
namespace io {

inline void decode(bool &b, Reader &reader) {
    b = reader.readBool();
}

template<class T>
inline typename std::enable_if<
    std::is_integral<T>::value &&
    !std::is_same<T, bool>::value
>::type
decode(T &v, Reader &reader) {
    v = reader.readInteger<T>();
}

}
} // hprose::io
