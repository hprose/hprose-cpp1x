/**********************************************************\
 *                                                        *
 * hprose/io/decoders/BoolDecoder.cpp                     *
 *                                                        *
 * hprose bool decoder for cpp.                           *
 *                                                        *
 * LastModified: Dec 12, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#include <hprose/io/decoders/BoolDecoder.h>
#include <hprose/io/Reader.h>

namespace hprose {
namespace io {
namespace decoders {

bool parseBool(const std::string &s) {
    if (s == "1" || s == "t" || s == "T" || s == "true" || s == "TRUE" || s == "True") {
        return true;
    }
    if (s == "0" || s == "f" || s == "F" || s == "false" || s == "FALSE" || s == "False") {
        return false;
    }
    throw std::runtime_error("parse bool failed");
}

bool readNumberAsBool(Reader &reader) {
    std::string str = reader.readUntil(TagSemicolon);
    if (str.length() == 1) {
        return str.at(0) != '0';
    } else {
        return true;
    }
}

inline bool readInfinityAsBool(Reader &reader) {
    reader.stream.ignore();
    return true;
}

inline bool readUTF8CharAsBool(Reader &reader) {
    return parseBool(reader.readUTF8String(1));
}

inline bool readStringAsBool(Reader &reader) {
    return parseBool(reader.readStringWithoutTag());
}

bool readRefAsBool(Reader &reader) {
    const auto &var = reader.readRef();
    if (var.isString()) {
        return parseBool(var.getString());
    }
    throw std::runtime_error(std::string("value of type ") + var.typeName() + " cannot be converted to type bool");
}

bool BoolDecode(Reader &reader, char tag) {
    switch (tag) {
        case '0':
        case TagNull:
        case TagEmpty:
        case TagFalse:    return false;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case TagTrue:
        case TagNaN:      return true;
        case TagInteger:
        case TagLong:
        case TagDouble:   return readNumberAsBool(reader);
        case TagInfinity: return readInfinityAsBool(reader);
        case TagUTF8Char: return readUTF8CharAsBool(reader);
        case TagString:   return readStringAsBool(reader);
        case TagRef:      return readRefAsBool(reader);
        default:          throw  CastError<bool>(tag);
    }
}

}
}
} // hprose::io::decoders
