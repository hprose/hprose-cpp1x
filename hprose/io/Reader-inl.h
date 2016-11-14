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
 * LastModified: Nov 14, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <deque>
#include <forward_list>
#include <list>
#include <map>

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

template<class Key, class Compare, class Allocator>
inline void decode(std::set<Key, Compare, Allocator> &v, Reader &reader) {
    reader.readList(v);
}

template<class Key, class Compare, class Allocator>
inline void decode(std::multiset<Key, Compare, Allocator> &v, Reader &reader) {
    reader.readList(v);
}

template<class Key, class Hash, class KeyEqual, class Allocator>
inline void decode(std::unordered_set<Key, Hash, KeyEqual, Allocator> &v, Reader &reader) {
    reader.readList(v);
}

template<class Key, class Hash, class KeyEqual, class Allocator>
inline void decode(std::unordered_multiset<Key, Hash, KeyEqual, Allocator> &v, Reader &reader) {
    reader.readList(v);
}

template<size_t N>
inline void decode(std::bitset<N> &v, Reader &reader) {
    reader.readList(v);
};

template<class... Types>
inline void decode(std::tuple<Types...> &v, Reader &reader) {
    reader.readList(v);
};

template<class Key, class T, class Compare, class Allocator>
inline void decode(std::map<Key, T, Compare, Allocator> &v, Reader &reader) {
    reader.readMap(v);
}

template<class Key, class T, class Compare, class Allocator>
inline void decode(std::multimap<Key, T, Compare, Allocator> &v, Reader &reader) {
    reader.readMap(v);
}

template<class Key, class T, class Hash, class KeyEqual, class Allocator>
inline void decode(std::unordered_map<Key, T, Hash, KeyEqual, Allocator> &v, Reader &reader) {
    reader.readMap(v);
}

template<class Key, class T, class Hash, class KeyEqual, class Allocator>
inline void decode(std::unordered_multimap<Key, T, Hash, KeyEqual, Allocator> &v, Reader &reader) {
    reader.readMap(v);
}

}
} // hprose::io
