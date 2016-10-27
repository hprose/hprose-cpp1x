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
 * hprose/util/Util.h                                     *
 *                                                        *
 * some util for cpp.                                     *
 *                                                        *
 * LastModified: Oct 27, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <ostream>
#include <string>
#include <type_traits>

namespace hprose {
namespace util {

void WriteInt(std::ostream &stream, int64_t i);

void WriteUint(std::ostream &stream, uint64_t u);

void WriteDate(std::ostream &stream, int year, int month, int day);

void WriteTime(std::ostream &stream, int hour, int min, int sec);

// UTF16Length return the UTF16 length of str.
// str must be an UTF8 encode string, otherwise return -1.
int UTF16Length(const std::string &str);

template<class T>
inline typename std::enable_if<
    std::is_same<T, float>::value,
    T
>::type
StringToFloat(const std::string &s) {
    return std::stof(s);
}

template<class T>
inline typename std::enable_if<
    std::is_same<T, double>::value,
    T
>::type
StringToFloat(const std::string &s) {
    return std::stod(s);
}

template<class T>
inline typename std::enable_if<
    std::is_same<T, long double>::value,
    T
>::type
StringToFloat(const std::string &s) {
    return std::stold(s);
}

}
} // hprose::util
