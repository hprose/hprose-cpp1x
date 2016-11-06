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
 * hprose/io/decoders/FloatDecoder-inl.h                  *
 *                                                        *
 * hprose float decoder for cpp.                          *
 *                                                        *
 * LastModified: Nov 6, 2016                              *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <limits>

namespace hprose {
namespace io {
namespace decoders {

template<class T>
inline typename std::enable_if<
    std::is_floating_point<T>::value,
    T
>::type
readLongAsFloat(Reader &reader) {
    return reader.readArithmetic<T>(tags::TagSemicolon);
}

template<class T>
inline typename std::enable_if<
    std::is_floating_point<T>::value,
    T
>::type
readUTF8CharAsFloat(Reader &reader) {
    return util::StringToFloat<T>(reader.readUTF8String(1));
}

template<class T>
inline typename std::enable_if<
    std::is_floating_point<T>::value,
    T
>::type
readStringAsFloat(Reader &reader) {
    return util::StringToFloat<T>(reader.readStringWithoutTag());
}

template<class T>
typename std::enable_if<
    std::is_floating_point<T>::value,
    T
>::type
readDateTimeAsFloat(Reader &reader) {
    auto tm = reader.readDateTimeWithoutTag();
    return mktime(&tm);
}

template<class T>
typename std::enable_if<
    std::is_floating_point<T>::value,
    T
>::type
readTimeAsFloat(Reader &reader) {
    auto tm = reader.readTimeWithoutTag();
    return mktime(&tm);
}

template<class T>
typename std::enable_if<
    std::is_floating_point<T>::value,
    T
>::type
readRefAsFloat(Reader &reader) {
    const auto &var = reader.readRef();
    if (var.isString()) {
        return util::StringToFloat<T>(var.getString());
    }
    if (var.isTime()) {
        auto tm = var.getTime();
        return mktime(&tm);
    }
    throw std::runtime_error(std::string("value of type ") + var.typeName() + " cannot be converted to type float");
}

template<class T>
typename std::enable_if<
    std::is_floating_point<T>::value,
    T
>::type
FloatDecode(Reader &reader, char tag) {
    switch (tag) {
        case '0':
        case tags::TagNull:
        case tags::TagEmpty:
        case tags::TagFalse:
            return 0;
        case '1':
        case tags::TagTrue:
            return 1;
        case '2':
            return 2;
        case '3':
            return 3;
        case '4':
            return 4;
        case '5':
            return 5;
        case '6':
            return 6;
        case '7':
            return 7;
        case '8':
            return 8;
        case '9':
            return 9;
        case tags::TagNaN:
            return std::numeric_limits<T>::quiet_NaN();
        case tags::TagInfinity:
            return reader.readInfinity<T>();
        case tags::TagInteger:
        case tags::TagLong:
            return readLongAsFloat<T>(reader);
        case tags::TagDouble:
            return reader.ByteReader::readFloat<T>();
        case tags::TagUTF8Char:
            return readUTF8CharAsFloat<T>(reader);
        case tags::TagString:
            return readStringAsFloat<T>(reader);
        case tags::TagDate:
            return readDateTimeAsFloat<T>(reader);
        case tags::TagTime:
            return readTimeAsFloat<T>(reader);
        case tags::TagRef:
            return readRefAsFloat<T>(reader);
        default:
            throw CastError<T>(tag);
    }
}

}
}
} // hprose::io::decoders
