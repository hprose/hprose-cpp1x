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
 * LastModified: Dec 26, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

namespace hprose {
namespace io {
namespace decoders {

template<class T>
void readMapAsObject(T &, Reader &) {
}

template<class T>
void readObject(T &v, Reader &reader) {
    auto index = reader.readCount();
    auto fields = reader.fieldsRef[index];
    reader.setRef(Ref(v));
    for (auto &field : fields) {
        field.decode(&v, reader);
    }
    reader.stream.ignore();
}

template<class T>
void readRefAsObject(T &v, Reader &reader) {
    const auto &var = reader.readRef();
        if (var.isRef()) {
        const Ref &ref = var.getRef();
        if (typeid(T) == *ref.type) {
            v = *static_cast<const T *>(ref.ptr);
            return;
        }
        throw std::runtime_error(std::string("value of type ") + ref.type->name() + " cannot be converted to type " + typeid(T).name());
    }
    throw std::runtime_error(std::string("value of type ") + var.typeName() + " cannot be converted to type " + typeid(T).name());
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
