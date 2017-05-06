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
 * LastModified: Dec 15, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <deque>
#include <list>
#include <set>
#include <unordered_set>
#include <map>
#include <stdexcept>

namespace hprose {
namespace io {

inline void encode(std::nullptr_t, Writer &writer) {
    writer.writeNull();
}

inline void encode(bool v, Writer &writer) {
    writer.writeBool(v);
}

template<class T>
inline typename std::enable_if<
    std::is_integral<T>::value &&
    !std::is_same<T, bool>::value
>::type
encode(T v, Writer &writer) {
    writer.writeInteger(v);
}

template<class T>
inline typename std::enable_if<
    std::is_enum<T>::value
>::type
encode(T v, Writer &writer) {
#ifdef HPROSE_HAS_UNDERLYING_TYPE
    writer.writeInteger(static_cast<typename std::underlying_type<T>::type>(v));
#else // HPROSE_HAS_UNDERLYING_TYPE
    writer.writeInteger(static_cast<int>(v));
#endif // HPROSE_HAS_UNDERLYING_TYPE
}

template<class T>
inline typename std::enable_if<
    std::is_floating_point<T>::value
>::type
encode(T v, Writer &writer) {
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

template<class T>
inline typename std::enable_if<
    std::is_same<T, char>::value ||
    std::is_same<T, wchar_t>::value ||
    std::is_same<T, char16_t>::value ||
    std::is_same<T, char32_t>::value
>::type
encode(const T *v, Writer &writer) {
    if (v) {
        writer.writeString(v);
    } else {
        writer.writeNull();
    }
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
    if (v) {
        writer.writeValue(*v);
    } else {
        writer.writeNull();
    }
}

template<class T, class Deleter>
inline void encode(const std::unique_ptr<T, Deleter> &v, Writer &writer) {
    if (v) {
        writer.writeValue(*v);
    } else {
        writer.writeNull();
    }
}

template<class T>
inline void encode(const std::shared_ptr<T> &v, Writer &writer) {
    if (v) {
        writer.writeValue(*v);
    } else {
        writer.writeNull();
    }
}

template<class T>
inline void encode(const std::weak_ptr<T> &v, Writer &writer) {
    if (auto spt = v.lock()) {
        writer.writeValue(*spt);
    } else {
        throw std::runtime_error("the weak pointer has expired");
    }
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
