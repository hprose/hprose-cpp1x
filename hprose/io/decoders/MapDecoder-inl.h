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
 * hprose/io/decoders/MapDecoder-inl.h                    *
 *                                                        *
 * hprose map decoder for cpp.                            *
 *                                                        *
 * LastModified: Dec 12, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

namespace hprose {
namespace io {
namespace decoders {

template<class T>
inline void setIntKey(T &v, int i) {
    throw std::runtime_error(std::string("cannot convert int to type ") + typeid(T).name());
}

template<class T>
inline typename std::enable_if<
    std::is_integral<T>::value &&
    !std::is_same<T, bool>::value
>::type
setIntKey(T &v, int i) {
    v = i;
}

inline void setIntKey(std::string &str, int i) {
    str = std::to_string(i);
}

template<class T>
void readListAsMap(T &v, Reader &reader) {
    auto count = reader.readCount();
    for (auto i = 0; i < count; ++i) {
        typename T::key_type key;
        setIntKey(key, i);
        typename T::mapped_type value;
        reader.readValue(value);
        v.insert({key, value});
    }
    reader.stream.ignore();
}

template<class T>
void readMap(T &v, Reader &reader) {
    auto count = reader.readCount();
    for (auto i = 0; i < count; ++i) {
        typename T::key_type key;
        reader.readValue(key);
        typename T::mapped_type value;
        reader.readValue(value);
        v.insert({key, value});
    }
    reader.stream.ignore();
}

template<class T>
void readClass(T &v, Reader &reader) {
    auto className = reader.ByteReader::readString();
    const auto &cache = ClassManager::SharedInstance().getClassCache(className);
    auto count = reader.readCount();
    for (auto i = 0; i < count; ++i) {
        auto fieldName = reader.readString<std::string>();
    }
    reader.stream.ignore();
    reader.readValue(v);
}

template<class T>
void readObjectAsMap(T &v, Reader &reader) {
}

template<class T>
void readRefAsMap(T &v, Reader &reader) {
}

template<class T>
inline void MapDecode(T &v, Reader &reader, char tag) {
    switch (tag) {
        case TagList:   readListAsMap(v, reader);   break;
        case TagMap:    readMap(v, reader);         break;
        case TagClass:  readClass(v, reader);       break;
        case TagObject: readObjectAsMap(v, reader); break;
        case TagRef:    readRefAsList(v, reader);   break;
        default:        throw CastError<T>(tag);
    }
}

}
}
} // hprose::io::decoders
