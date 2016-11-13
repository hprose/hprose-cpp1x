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

namespace hprose {
namespace io {
namespace decoders {

template<class T>
void readBytesAsList(T &v, Reader &reader) {
    auto len = reader.readLength();
}

template<class T>
void readList(T &v, Reader &reader) {
    auto count = reader.readCount();
    v.resize(count);
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
        default: throw CastError<T>(tag);
    }
}

}
}
} // hprose::io::decoders
