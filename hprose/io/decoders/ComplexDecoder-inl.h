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
 * hprose/io/decoders/ComplexDecoder-inl.h                *
 *                                                        *
 * hprose float decoder for cpp.                          *
 *                                                        *
 * LastModified: Dec 30, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <limits>

namespace hprose {
namespace io {
namespace decoders {

template<class T>
void readLongAsComplex(std::complex<T> &v, Reader &reader) {
    v = std::complex<T>(reader.readArithmetic<T>(TagSemicolon));
}

template<class T>
void readUTF8CharAsComplex(std::complex<T> &v, Reader &reader) {
    v = std::complex<T>(util::StringToFloat<T>(reader.readUTF8String(1)));
}

template<class T>
void readStringAsComplex(std::complex<T> &v, Reader &reader) {
    v = std::complex<T>(util::StringToFloat<T>(reader.readStringWithoutTag()));
}

template<class T>
void readList(T &v, Reader &reader);

template<class T>
void readListAsComplex(std::complex<T> &v, Reader &reader) {
    std::array<T, 2> pair;
    readList(pair, reader);
    v = std::complex<T>(pair[0], pair[1]);
}

template<class T>
void readRefAsComplex(std::complex<T> &v, Reader &reader) {
    const auto &var = reader.readRef();
    if (var.isString()) {
        v = std::complex<T>(util::StringToFloat<T>(var.getString()));
        return;
    }
    if (var.isRef()) {
        const Ref &ref = var.getRef();
        if (typeid(std::array<T, 2>) == *ref.type) {
            std::array<T, 2> pair = *static_cast<const std::array<T, 2> *>(ref.ptr);
            v = std::complex<T>(pair[0], pair[1]);
            return;
        }
        throw std::runtime_error(std::string("value of type ") + ref.type->name() + " cannot be converted to type " + typeid(T).name());
    }
    throw std::runtime_error(std::string("value of type ") + var.typeName() + " cannot be converted to type complex");
}

template<class T>
void ComplexDecode(std::complex<T> &v, Reader &reader, char tag) {
    switch (tag) {
        case '0':
        case TagNull:
        case TagEmpty:
        case TagFalse:    v = std::complex<T>(); break;
        case '1':
        case TagTrue:     v = std::complex<T>(1); break;
        case '2':         v = std::complex<T>(2); break;
        case '3':         v = std::complex<T>(3); break;
        case '4':         v = std::complex<T>(4); break;
        case '5':         v = std::complex<T>(5); break;
        case '6':         v = std::complex<T>(6); break;
        case '7':         v = std::complex<T>(7); break;
        case '8':         v = std::complex<T>(8); break;
        case '9':         v = std::complex<T>(9); break;
        case TagNaN:      v = std::complex<T>(std::numeric_limits<T>::quiet_NaN()); break;
        case TagInfinity: v = std::complex<T>(reader.readInfinity<T>()); break;
        case TagInteger:
        case TagLong:     readLongAsComplex(v, reader); break;
        case TagDouble:   v = std::complex<T>(reader.ByteReader::readFloat<T>()); break;
        case TagUTF8Char: readUTF8CharAsComplex(v, reader); break;
        case TagString:   readStringAsComplex(v, reader); break;
        case TagList:     readListAsComplex(v, reader); break;
        case TagRef:      readRefAsComplex(v, reader); break;
        default:          throw CastError<T>(tag);
    }
}

}
}
} // hprose::io::decoders
