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
 * LastModified: Oct 21, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

namespace hprose {
namespace io {

template<class T>
inline void decode(T &, Reader &reader) {
}

template<>
inline void decode(bool &b, Reader &reader) {
    b = reader.readBool();
}

}
} // hprose::io
