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
 * LastModified: Dec 26, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

namespace hprose {
namespace io {
namespace decoders {

template<class T>
inline void setIntKey(T &, int) {
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
    reader.setRef(Ref(v));
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
    reader.setRef(Ref(v));
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
    auto classManager = ClassManager::SharedInstance();
    auto classType = classManager.getClassType(className);
    if (!classType) {
        throw std::runtime_error("cannot convert " + className + " to type " + typeid(T).name());
    }
    const auto &classCache = classManager.getClassCache(*classType);
    auto fieldMap = classCache.fieldMap;
    auto count = reader.readCount();
    std::vector<FieldCache> fields;
    for (auto i = 0; i < count; ++i) {
        auto fieldName = reader.readString<std::string>();
        fields.push_back(fieldMap[fieldName]);
    }
    reader.fieldsRef.push_back(fields);
    reader.stream.ignore();
    reader.readValue(v);
}

template<class T>
void readObjectAsMap(T &v, Reader &reader) {
    auto index = reader.readCount();
    auto fields = reader.fieldsRef[index];
    reader.setRef(Ref(v));
    for (auto &field : fields) {
        v[field.alias] = reader.unserialize<typename T::mapped_type>();
    }
    reader.stream.ignore();
}

template<class T>
void readRefAsMap(T &v, Reader &reader) {
    const auto &var = reader.readRef();
    if (var.isRef()) {
        const Ref &ref = var.getRef();
        if (typeid(T) == *ref.type) {
            v = *static_cast<const T *>(ref.ptr);
            return;
        }
        throw std::runtime_error(std::string("value of type ") + ref.type->name() + " cannot be converted to type " + typeid(T).name());
    }
    throw std::runtime_error(std::string("value of type ") + var.typeName() + " cannot be converted to type map");
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
