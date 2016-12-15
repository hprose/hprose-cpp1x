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
 * hprose/io/Formatter.h                                  *
 *                                                        *
 * hprose formatter for cpp.                              *
 *                                                        *
 * LastModified: Dec 15, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <hprose/io/Writer.h>
#include <hprose/io/Reader.h>

namespace hprose {
namespace io {

template<class T>
inline std::string serialize(const T &v, bool simple = false) {
    std::ostringstream stream;
    Writer(stream, simple).writeValue(v);
    return stream.str();
}

template<class T>
inline T unserialize(const std::istream &stream, bool simple = false) {
    return Reader(stream, simple).unserialize();
}

template<class T>
inline T unserialize(const std::string &data, bool simple = false) {
    std::istringstream stream(data);
    return Reader(stream, simple).unserialize();
}

template<class T>
inline void &unserialize(const std::istream &stream, T &v, bool simple = false) {
    Reader(stream, simple).readValue(v);
}

template<class T>
inline void &unserialize(const std::string &data, T &v, bool simple = false) {
    std::istringstream stream(data);
    Reader(stream, simple).readValue(v);
}

}
} // hprose::io
