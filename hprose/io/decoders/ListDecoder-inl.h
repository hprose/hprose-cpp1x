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
 * hprose/io/decoders/ListDecoder-inl.h                   *
 *                                                        *
 * hprose list decoder for cpp.                           *
 *                                                        *
 * LastModified: Dec 26, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <array>
#include <vector>
#include <set>
#include <unordered_set>
#include <bitset>
#include <tuple>
#include <hprose/util/Util.h>

namespace hprose {
namespace io {
namespace decoders {

inline void checkSize(size_t actual, int expected) {
    if (static_cast<size_t>(expected) != actual)
        throw std::runtime_error("expected array size " + std::to_string(expected)
            + ", actual size " + std::to_string(expected));    
}

template<class T>
inline void makeSize(T &v, int count) {
    v.resize(count);
}

template<class T, size_t N>
inline void makeSize(T (&v)[N], int count) {
    (void)v;
    checkSize(N, count);
}

template<class T, size_t N>
inline void makeSize(std::array<T, N> &, int count) {
    checkSize(N, count);
}

template<class T>
void readBytes(T &v, Reader &reader) {
    auto len = reader.readLength();
    auto str = reader.read(len);
    makeSize(v, len);
    std::copy(str.cbegin(), str.cend(), &v[0]); // Todo: avoid copy
    reader.stream.ignore();
}

template<class T>
void readList(T &v, Reader &reader) {
    auto count = reader.readCount();
    makeSize(v, count);
    reader.setRef(Ref(v));
    for(auto &e : v) {
        reader.readValue(e);
    }
    reader.stream.ignore();
}

template<class T>
void readListAsSet(T &v, Reader &reader) {
    auto count = reader.readCount();
    for(auto i = 0; i < count; ++i) {
        typename T::key_type key;
        reader.readValue(key);
        v.insert(std::move(key));
    }
    reader.stream.ignore();
}

template<class Key, class Compare, class Allocator>
inline void readList(std::set<Key, Compare, Allocator> &v, Reader &reader) {
    readListAsSet(v, reader);
}

template<class Key, class Compare, class Allocator>
inline void readList(std::multiset<Key, Compare, Allocator> &v, Reader &reader) {
    readListAsSet(v, reader);
}

template<class Key, class Hash, class KeyEqual, class Allocator>
inline void readList(std::unordered_set<Key, Hash, KeyEqual, Allocator> &v, Reader &reader) {
    readListAsSet(v, reader);
}

template<class Key, class Hash, class KeyEqual, class Allocator>
inline void readList(std::unordered_multiset<Key, Hash, KeyEqual, Allocator> &v, Reader &reader) {
    readListAsSet(v, reader);
}

template<size_t N>
inline void readList(std::bitset<N> &v, Reader &reader) {
    auto count = reader.readCount();
    checkSize(N, count);
    for(auto i = 0; i < count; ++i) {
        v.set(i, reader.unserialize<bool>());
    }
    reader.stream.ignore();   
};

template<std::size_t Index = 0, class... Tuple>
inline typename std::enable_if<
    Index == sizeof...(Tuple)
>::type
readTupleElement(std::tuple<Tuple...> &, Reader &) {
}

template<std::size_t Index = 0, class... Tuple>
inline typename std::enable_if<
    Index < sizeof...(Tuple)
>::type
readTupleElement(std::tuple<Tuple...> &tuple, Reader &reader) {
    reader.readValue(std::get<Index>(tuple));
    readTupleElement<Index + 1, Tuple...>(tuple, reader);
}

template<class... Type>
void readList(std::tuple<Type...> &lst, Reader &reader) {
    auto count = reader.readCount();
    checkSize(std::tuple_size<std::tuple<Type...> >::value, count);
    readTupleElement(lst, reader);
    reader.stream.ignore(); 
}

template<class T>
void readRefAsList(T &v, Reader &reader) {
    const auto &var = reader.readRef();
    if (var.isRef()) {
        const Ref &ref = var.getRef();
        if (typeid(T) == *ref.type) {
            v = *static_cast<const T *>(ref.ptr);
            return;
        }
        throw std::runtime_error(std::string("value of type ") + ref.type->name() + " cannot be converted to type " + typeid(T).name());
    }
    throw std::runtime_error(std::string("value of type ") + var.typeName() + " cannot be converted to type list");
}

namespace detail {

template<class T>
void ListDecode(T &v, Reader &reader, char tag) {
    switch (tag) {
        case TagList:  readList(v, reader);      break;
        case TagRef:   readRefAsList(v, reader); break;
        default:       throw CastError<T>(tag);
    }
}

template<class T, size_t N>
void ListDecode(T (&v)[N], Reader &reader, char tag) {
    switch (tag) {
        case TagList:  readList(v, reader);      break;
        default:       throw CastError<uint8_t[N]>(tag);
    }
}

template<size_t N>
void ListDecode(uint8_t (&v)[N], Reader &reader, char tag) {
    switch (tag) {
        case TagBytes: readBytes(v, reader);     break;
        case TagList:  readList(v, reader);      break;
        default:       throw CastError<uint8_t[N]>(tag);
    }
}

template<size_t N>
void ListDecode(std::array<uint8_t, N> &v, Reader &reader, char tag) {
    switch (tag) {
        case TagBytes: readBytes(v, reader);     break;
        case TagList:  readList(v, reader);      break;
        case TagRef:   readRefAsList(v, reader); break;
        default:       throw CastError<std::array<uint8_t, N> >(tag);
    }
}

template<class Allocator>
void ListDecode(std::vector<uint8_t, Allocator> &v, Reader &reader, char tag) {
    switch (tag) {
        case TagBytes: readBytes(v, reader);     break;
        case TagList:  readList(v, reader);      break;
        case TagRef:   readRefAsList(v, reader); break;
        default:       throw CastError<std::vector<uint8_t, Allocator> >(tag);
    }
}

} // detail

template<class T>
inline void ListDecode(T &v, Reader &reader, char tag) {
    detail::ListDecode(v, reader, tag);
}

}
}
} // hprose::io::decoders
