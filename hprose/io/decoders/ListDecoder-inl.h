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
 * LastModified: Nov 14, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <array>
#include <vector>

namespace hprose {
namespace io {
namespace decoders {

template<class T>
inline void checkSize(T &v, int count) {
    v.resize(count);
}

template<class T, size_t N>
inline void checkSize(T (&v)[N], int count) {
    if (N != count) throw std::runtime_error("expected array size " + std::to_string(count) + ", actual size " + std::to_string(N));
}

template<class T, size_t N>
inline void checkSize(std::array<T, N> &v, int count) {
    if (N != count) throw std::runtime_error("expected array size " + std::to_string(count) + ", actual size " + std::to_string(N));
}

template<class T>
void readBytes(T &v, Reader &reader) {
    auto len = reader.readLength();
    auto str = reader.read(len);
    checkSize(v, len);
    std::copy(str.cbegin(), str.cend(), &v[0]); // Todo: avoid copy
    reader.stream.ignore();
}

template<class T>
void readList(T &v, Reader &reader) {
    auto count = reader.readCount();
    checkSize(v, count);
    for(auto &e : v) {
        reader.readValue(e);
    }
    reader.stream.ignore();
}

template<class T>
void readRefAsList(T &v, Reader &reader) {
    
}

namespace detail {

template<class T>
void ListDecode(T &v, Reader &reader, char tag) {
    switch (tag) {
        case tags::TagList:  readList(v, reader);      break;
        case tags::TagRef:   readRefAsList(v, reader); break;
        default:             throw CastError<T>(tag);
    }
}

template<size_t N>
void ListDecode(uint8_t (&v)[N], Reader &reader, char tag) {
    switch (tag) {
        case tags::TagBytes: readBytes(v, reader);     break;
        case tags::TagList:  readList(v, reader);      break;
        case tags::TagRef:   readRefAsList(v, reader); break;
        default:             throw CastError<uint8_t[N]>(tag);
    }
}

template<size_t N>
void ListDecode(std::array<uint8_t, N> &v, Reader &reader, char tag) {
    switch (tag) {
        case tags::TagBytes: readBytes(v, reader);     break;
        case tags::TagList:  readList(v, reader);      break;
        case tags::TagRef:   readRefAsList(v, reader); break;
        default:             throw CastError<std::array<uint8_t, N> >(tag);
    }
}

template<class Allocator>
void ListDecode(std::vector<uint8_t, Allocator> &v, Reader &reader, char tag) {
    switch (tag) {
        case tags::TagBytes: readBytes(v, reader);     break;
        case tags::TagList:  readList(v, reader);      break;
        case tags::TagRef:   readRefAsList(v, reader); break;
        default:             throw CastError<std::vector<uint8_t, Allocator> >(tag);
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
