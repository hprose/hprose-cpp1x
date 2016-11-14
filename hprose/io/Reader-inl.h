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
 * LastModified: Nov 15, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <deque>
#include <forward_list>
#include <list>

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

template<class T>
inline typename std::enable_if<
    std::is_floating_point<T>::value
>::type
decode(T &v, Reader &reader) {
    v = reader.readFloat<T>();
}

template<class Element, class Traits, class Allocator>
inline void decode(std::basic_string<Element, Traits, Allocator> &v, Reader &reader) {
    v = reader.readString<std::basic_string<Element, Traits, Allocator> >();
}

template<class T>
inline typename std::enable_if<
    std::is_array<T>::value
>::type
decode(T &v, Reader &reader) {
    reader.readList(v);
}

template<class T, size_t N>
inline void decode(std::array<T, N> &v, Reader &reader) {
    reader.readList(v);
}

template<class T, class Allocator>
inline void decode(std::vector<T, Allocator> &v, Reader &reader) {
    reader.readList(v);
}

template<class T, class Allocator>
inline void decode(std::deque<T, Allocator> &v, Reader &reader) {
    reader.readList(v);
}

template<class T, class Allocator>
inline void decode(std::forward_list<T, Allocator> &v, Reader &reader) {
    reader.readList(v);
}

template<class T, class Allocator>
inline void decode(std::list<T, Allocator> &v, Reader &reader) {
    reader.readList(v);
}

}
} // hprose::io
