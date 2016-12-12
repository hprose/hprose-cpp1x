/**********************************************************\
 *                                                        *
 * hprose/io/decoders/IntDecoder.cpp                      *
 *                                                        *
 * hprose int decoder for cpp.                            *
 *                                                        *
 * LastModified: Dec 12, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#include <hprose/io/decoders/IntDecoder.h>
#include <hprose/io/Reader.h>

namespace hprose {
namespace io {
namespace decoders {

inline int64_t readInt64(Reader &reader) {
    return reader.readArithmetic<int64_t>(TagSemicolon);
}

inline int64_t readDoubleAsInt(Reader &reader) {
    return static_cast<int64_t>(reader.ByteReader::readFloat<double>());
}

inline int64_t readUTF8CharAsInt(Reader &reader) {
    return std::stoll(reader.readUTF8String(1));
}

inline int64_t readStringAsInt(Reader &reader) {
    return std::stoll(reader.readStringWithoutTag());
}

int64_t readDateTimeAsInt(Reader &reader) {
    auto tm = reader.readDateTimeWithoutTag();
    return mktime(&tm);
}

int64_t readTimeAsInt(Reader &reader) {
    auto tm = reader.readTimeWithoutTag();
    return mktime(&tm);
}

int64_t readRefAsInt(Reader &reader) {
    const auto &var = reader.readRef();
    if (var.isString()) {
        return std::stoll(var.getString());
    }
    if (var.isTime()) {
        auto tm = var.getTime();
        return mktime(&tm);
    }
    throw std::runtime_error(std::string("value of type ") + var.typeName() + " cannot be converted to type int");
}

int64_t IntDecode(Reader &reader, char tag) {
    switch (tag) {
        case '0':
        case TagNull:
        case TagEmpty:
        case TagFalse:    return 0;
        case '1':
        case TagTrue:     return 1;
        case '2':         return 2;
        case '3':         return 3;
        case '4':         return 4;
        case '5':         return 5;
        case '6':         return 6;
        case '7':         return 7;
        case '8':         return 8;
        case '9':         return 9;
        case TagInteger:
        case TagLong:     return readInt64(reader);
        case TagDouble:   return readDoubleAsInt(reader);
        case TagUTF8Char: return readUTF8CharAsInt(reader);
        case TagString:   return readStringAsInt(reader);
        case TagDate:     return readDateTimeAsInt(reader);
        case TagTime:     return readTimeAsInt(reader);
        case TagRef:      return readRefAsInt(reader);
        default:          throw  CastError<int64_t>(tag);
    }
}

}
}
} // hprose::io::decoders
