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
 * LastModified: Nov 15, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

namespace hprose {
namespace io {
namespace decoders {

template<class T>
void readBytesAsList(T &v, Reader &reader) {
    auto len = reader.readLength();
}

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

template<class T>
void ListDecode(T &v, Reader &reader, char tag) {
    switch (tag) {
        case tags::TagBytes: readBytesAsList(v, reader); break;
        case tags::TagList:  readList(v, reader);        break;
        case tags::TagRef:   readRefAsList(v, reader);   break;
        default:             throw CastError<T>(tag);
    }
}

}
}
} // hprose::io::decoders
