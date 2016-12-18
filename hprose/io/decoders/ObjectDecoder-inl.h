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
 * hprose/io/decoders/ObjectDecoder-inl.h                 *
 *                                                        *
 * hprose object decoder for cpp.                         *
 *                                                        *
 * LastModified: Dec 18, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

namespace hprose {
namespace io {
namespace decoders {

template<class T>
void readMapAsObject(T &v, Reader &reader) {
}

template<class T>
void readObject(T &v, Reader &reader) {
}

template<class T>
void readRefAsObject(T &v, Reader &reader) {
}

template<class T>
inline void ObjectDecode(T &v, Reader &reader, char tag) {
    switch (tag) {
        case TagMap:    readMapAsObject(v, reader);       break;
        case TagClass:  readClass(v, reader);     break;
        case TagObject: readObject(v, reader);    break;
        case TagRef:    readRefAsObject(v, reader); break;
        default:        throw CastError<T>(tag);
    }
}

}
}
} // hprose::io::decoders
