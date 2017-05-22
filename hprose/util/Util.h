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

#include <hprose/util/Config.h>
#include <ostream>
#include <string>
#include <type_traits>

#ifndef HPROSE_HAS_STOX
#include <cstdio>
#include <cstdlib>
namespace std {
inline int stoi(const std::string &str, std::size_t* pos = 0, int base = 10) {
    char *ptr;
    auto ret = strtol(str.c_str(), &ptr, base);
    if (pos != 0) {
        *pos = ptr - str.c_str();
    }
    return ret;
}
inline unsigned long stoul(const std::string &str, std::size_t* pos = 0, int base = 10) {
    char *ptr;
    auto ret = strtoul(str.c_str(), &ptr, base);
    if (pos != 0) {
        *pos = ptr - str.c_str();
    }
    return ret;
}
inline long long stoll(const std::string &str, std::size_t* pos = 0, int base = 10) {
    char *ptr;
    auto ret = strtoll(str.c_str(), &ptr, base);
    if (pos != 0) {
        *pos = ptr - str.c_str();
    }
    return ret;
}
inline float stof(const std::string &str, std::size_t* pos = 0) {
    char *ptr;
    auto ret = strtof(str.c_str(), &ptr);
    if (pos != 0) {
        *pos = ptr - str.c_str();
    }
    return ret;
}
inline double stod(const std::string &str, std::size_t* pos = 0) {
    char *ptr;
    auto ret = strtod(str.c_str(), &ptr);
    if (pos != 0) {
        *pos = ptr - str.c_str();
    }
    return ret;
}
inline long double stold(const std::string &str, std::size_t* pos = 0) {
    char *ptr;
    auto ret = strtold(str.c_str(), &ptr);
    if (pos != 0) {
        *pos = ptr - str.c_str();
    }
    return ret;
}

inline std::string to_string(int value) {
    char szBuf[4 * sizeof(int)];
    const int nRet = snprintf(szBuf, sizeof(szBuf), "%d", value);
    return std::string(szBuf, nRet);
}

inline std::string to_string(long value) {
    char szBuf[4 * sizeof(long)];
    const int nRet = snprintf(szBuf, sizeof(szBuf), "%ld", value);
    return std::string(szBuf, nRet);
}

inline std::string to_string(long long value) {
    char szBuf[4 * sizeof(long long)];
    const int nRet = snprintf(szBuf, sizeof(szBuf), "%lld", value);
    return std::string(szBuf, nRet);
}

inline std::string to_string(unsigned value) {
    char szBuf[4 * sizeof(unsigned)];
    const int nRet = snprintf(szBuf, sizeof(szBuf), "%u", value);
    return std::string(szBuf, nRet);
}

inline std::string to_string(unsigned long value) {
    char szBuf[4 * sizeof(unsigned long)];
    const int nRet = snprintf(szBuf, sizeof(szBuf), "%lu", value);
    return std::string(szBuf, nRet);
}

inline std::string to_string(unsigned long long value) {
    char szBuf[4 * sizeof(unsigned long long)];
    const int nRet = snprintf(szBuf, sizeof(szBuf), "%llu", value);
    return std::string(szBuf, nRet);
}

inline std::string to_string(float value) {
    char szBuf[__FLT_MAX_10_EXP__ + 20];
    const int nRet = snprintf(szBuf, sizeof(szBuf), "%f", value);
    return std::string(szBuf, nRet);
}

inline std::string to_string(double value) {
    char szBuf[__DBL_MAX_10_EXP__ + 20];
    const int nRet = snprintf(szBuf, sizeof(szBuf), "%f", value);
    return std::string(szBuf, nRet);
}
} // namespace std
#endif // HPROSE_HAS_STOX

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
