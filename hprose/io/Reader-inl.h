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
 * LastModified: Dec 30, 2016                             *
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
    std::is_enum<T>::value
>::type
decode(T &v, Reader &reader) {
#ifdef HPROSE_HAS_UNDERLYING_TYPE
    v = static_cast<T>(reader.readInteger<typename std::underlying_type<T>::type>());
#else // HPROSE_HAS_UNDERLYING_TYPE
    v = static_cast<T>(reader.readInteger<int>());
#endif // HPROSE_HAS_UNDERLYING_TYPE
}

template<class T>
inline typename std::enable_if<
    std::is_floating_point<T>::value
>::type
decode(T &v, Reader &reader) {
    v = reader.readFloat<T>();
}

template<class T>
inline void decode(std::complex<T> &v, Reader &reader) {
    reader.readComplex(v);
}

template<class T>
inline typename std::enable_if<
    std::is_same<T, char>::value ||
    std::is_same<T, wchar_t>::value ||
    std::is_same<T, char16_t>::value ||
    std::is_same<T, char32_t>::value
>::type
decode(T *&v, Reader &reader) {
    char tag = static_cast<char>(reader.stream.peek());
    if (tag == TagNull) {
        v = nullptr;
    } else {
        auto str = reader.readString<std::basic_string<T>>();
        v = static_cast<T *>(malloc((str.size() + 1) * sizeof(T)));
        std::copy(str.begin(), str.end(), v);
        *(v + str.size()) = 0;
    }
}

template<class Element, class Traits, class Allocator>
inline void decode(std::basic_string<Element, Traits, Allocator> &v, Reader &reader) {
    v = reader.readString<std::basic_string<Element, Traits, Allocator>>();
}

template<class T>
inline typename std::enable_if<
    std::is_pointer<T>::value
>::type
decode(T &v, Reader &reader) {
    char tag = static_cast<char>(reader.stream.peek());
    if (tag == TagNull) {
        v = nullptr;
    } else if (tag == TagRef) {
        reader.readValue(*v);
    } else {
        v = new typename std::remove_pointer<T>::type();
        reader.readValue(*v);
    }
}

template<class T, class Deleter>
inline void decode(std::unique_ptr<T, Deleter> &v, Reader &reader) {
    char tag = static_cast<char>(reader.stream.peek());
    if (tag == TagNull) {
        v = nullptr;
    } else {
        v.reset(new T());
        reader.readValue(*v);
    }
}

template<class T>
inline void decode(std::shared_ptr<T> &v, Reader &reader) {
    char tag = static_cast<char>(reader.stream.peek());
    if (tag == TagNull) {
        v = nullptr;
    } else {
        v.reset(new T());
        reader.readValue(*v);
    }
}

template<class T>
inline void decode(std::weak_ptr<T> &, Reader &) {
    throw std::runtime_error("the weak pointer cannot be unserialized");
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
