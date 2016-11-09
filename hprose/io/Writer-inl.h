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
 * hprose/io/Writer-inl.h                                 *
 *                                                        *
 * hprose encode funtions for cpp.                        *
 *                                                        *
 * LastModified: Nov 9, 2016                              *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <deque>
#include <list>
#include <set>
#include <unordered_set>
#include <map>

namespace hprose {
namespace io {

inline void encode(const std::nullptr_t &v, Writer &writer) {
    writer.writeNull();
}

inline void encode(const bool &v, Writer &writer) {
    writer.writeBool(v);
}

template<class T>
inline typename std::enable_if<
    std::is_integral<T>::value &&
    !std::is_same<T, bool>::value
>::type
encode(const T &v, Writer &writer) {
    writer.writeInteger(v);
}

template<class T>
inline typename std::enable_if<
    std::is_enum<T>::value
>::type
encode(const T &v, Writer &writer) {
    writer.writeInteger(static_cast<int>(v));
}

template<class T>
inline typename std::enable_if<
    std::is_floating_point<T>::value
>::type
encode(const T &v, Writer &writer) {
    writer.writeFloat(v);
}

template<class T>
inline void encode(const std::complex<T> &v, Writer &writer) {
    writer.writeComplex(v);
}

template<intmax_t N, intmax_t D>
inline void encode(const std::ratio<N, D> &v, Writer &writer) {
    writer.writeRatio(v);
}

inline void encode(const char *v, Writer &writer) {
    writer.writeString(v);
}

inline void encode(const wchar_t *v, Writer &writer) {
    writer.writeString(v);
}

inline void encode(const char16_t *v, Writer &writer) {
    writer.writeString(v);
}

inline void encode(const char32_t *v, Writer &writer) {
    writer.writeString(v);
}

template<class Element, class Traits, class Allocator>
inline void encode(const std::basic_string<Element, Traits, Allocator> &v, Writer &writer) {
    writer.writeString(v);
}

inline void encode(const std::tm &v, Writer &writer) {
    writer.writeTime(v);
}

template<class Clock, class Duration>
inline void encode(const std::chrono::time_point<Clock, Duration> &v, Writer &writer) {
    writer.writeTime(v);
}

template<class T>
inline typename std::enable_if<
    std::is_pointer<T>::value
>::type
encode(const T &v, Writer &writer) {
    writer.writeValue(*v);
}

template<class T>
inline typename std::enable_if<
    std::is_array<T>::value
>::type
encode(const T &v, Writer &writer) {
    writer.writeList(v);
}

template<class T, size_t N>
inline void encode(const std::array<T, N> &v, Writer &writer) {
    writer.writeList(v);
}

template<class T, class Allocator>
inline void encode(const std::vector<T, Allocator> &v, Writer &writer) {
    writer.writeList(v);
}

template<class T, class Allocator>
inline void encode(const std::deque<T, Allocator> &v, Writer &writer) {
    writer.writeList(v);
}

template<class T, class Allocator>
inline void encode(const std::forward_list<T, Allocator> &v, Writer &writer) {
    writer.writeList(v);
}

template<class T, class Allocator>
inline void encode(const std::list<T, Allocator> &v, Writer &writer) {
    writer.writeList(v);
}

template<class Key, class Compare, class Allocator>
inline void encode(const std::set<Key, Compare, Allocator> &v, Writer &writer) {
    writer.writeList(v);
}

template<class Key, class Compare, class Allocator>
inline void encode(const std::multiset<Key, Compare, Allocator> &v, Writer &writer) {
    writer.writeList(v);
}

template<class Key, class Hash, class KeyEqual, class Allocator>
inline void encode(const std::unordered_set<Key, Hash, KeyEqual, Allocator> &v, Writer &writer) {
    writer.writeList(v);
}

template<class Key, class Hash, class KeyEqual, class Allocator>
inline void encode(const std::unordered_multiset<Key, Hash, KeyEqual, Allocator> &v, Writer &writer) {
    writer.writeList(v);
}

template<size_t N>
inline void encode(const std::bitset<N> &v, Writer &writer) {
    writer.writeList(v);
};

template<class... Types>
inline void encode(const std::tuple<Types...> &v, Writer &writer) {
    writer.writeList(v);
};

template<class Key, class T, class Compare, class Allocator>
inline void encode(const std::map<Key, T, Compare, Allocator> &v, Writer &writer) {
    writer.writeMap(v);
}

template<class Key, class T, class Compare, class Allocator>
inline void encode(const std::multimap<Key, T, Compare, Allocator> &v, Writer &writer) {
    writer.writeMap(v);
}

template<class Key, class T, class Hash, class KeyEqual, class Allocator>
inline void encode(const std::unordered_map<Key, T, Hash, KeyEqual, Allocator> &v, Writer &writer) {
    writer.writeMap(v);
}

template<class Key, class T, class Hash, class KeyEqual, class Allocator>
inline void encode(const std::unordered_multimap<Key, T, Hash, KeyEqual, Allocator> &v, Writer &writer) {
    writer.writeMap(v);
}

}
} // hprose::io
