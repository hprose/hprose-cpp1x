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
 * hprose/io/decoders/StringDecoder.cpp                   *
 *                                                        *
 * hprose string decoder for cpp.                         *
 *                                                        *
 * LastModified: Dec 12, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#include <hprose/io/decoders/StringDecoder.h>
#include <hprose/io/Reader.h>

namespace hprose {
namespace io {
namespace decoders {

inline std::string readInfAsString(Reader &reader) {
    return reader.stream.get() == TagPos ? "+Inf" : "-Inf";
}

inline std::string readNumberAsString(Reader &reader) {
    return reader.readUntil(TagSemicolon);
}

inline std::string readUTF8CharAsString(Reader &reader) {
    return reader.readUTF8String(1);
}

std::string readBytesAsString(Reader &reader) {
    auto len = reader.readLength();
    auto s = reader.read(len);
    reader.setRef(s);
    return s;
}

std::string readGUIDAsString(Reader &reader) {
    reader.stream.ignore();
    auto s = reader.read(36);
    reader.stream.ignore();
    reader.setRef(s);
    return s;
}

inline std::string readDateTimeAsString(Reader &reader) {
    auto tm = reader.readDateTimeWithoutTag();
    return std::asctime(&tm);
}

inline std::string readTimeAsString(Reader &reader) {
    auto tm = reader.readTimeWithoutTag();
    return std::asctime(&tm);
}

std::string readRefAsString(Reader &reader) {
    const auto &var = reader.readRef();
    if (var.isString()) {
        return var.getString();
    }
    if (var.isTime()) {
        auto tm = var.getTime();
        return std::asctime(&tm);
    }
    throw std::runtime_error(std::string("value of type ") + var.typeName() + " cannot be converted to type string");
}

std::string StringDecode(Reader &reader, char tag) {
    switch (tag) {
        case '0':         return "0";
        case '1':         return "1";
        case '2':         return "2";
        case '3':         return "3";
        case '4':         return "4";
        case '5':         return "5";
        case '6':         return "6";
        case '7':         return "7";
        case '8':         return "8";
        case '9':         return "9";
        case TagNull:
        case TagEmpty:    return "";
        case TagFalse:    return "false";
        case TagTrue:     return "true";
        case TagInfinity: return readInfAsString(reader);
        case TagInteger:
        case TagLong:
        case TagDouble:   return readNumberAsString(reader);
        case TagUTF8Char: return readUTF8CharAsString(reader);
        case TagString:   return reader.readStringWithoutTag();
        case TagBytes:    return readBytesAsString(reader);
        case TagGUID:     return readGUIDAsString(reader);
        case TagDate:     return readDateTimeAsString(reader);
        case TagTime:     return readTimeAsString(reader);
        case TagRef:      return readRefAsString(reader);
        default:          throw  CastError<std::string>(tag);
    }
}

}
}
} // hprose::io::decoders
