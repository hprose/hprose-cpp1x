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
 * hprose/io/Reader.cpp                                   *
 *                                                        *
 * hprose reader unit for cpp.                            *
 *                                                        *
 * LastModified: Nov 14, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#include <hprose/io/Reader.h>

namespace hprose {
namespace io {

namespace internal {

std::string TagToString(char tag) {
    switch (tag) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case tags::TagInteger:  return "int";
        case tags::TagLong:     return "long long";
        case tags::TagDouble:   return "double";
        case tags::TagNull:     return "null";
        case tags::TagEmpty:    return "empty string";
        case tags::TagTrue:     return "true";
        case tags::TagFalse:    return "false";
        case tags::TagNaN:      return "nan";
        case tags::TagInfinity: return "infinity";
        case tags::TagDate:
        case tags::TagTime:     return "time";
        case tags::TagBytes:    return "bytes";
        case tags::TagUTF8Char:
        case tags::TagString:   return "string";
        case tags::TagGUID:     return "uuid";
        case tags::TagList:     return "list";
        case tags::TagMap:      return "map";
        case tags::TagClass:    return "class";
        case tags::TagObject:   return "object";
        case tags::TagRef:      return "reference";
        default:                throw  UnexpectedTag(tag);
    }

}

}  // internal

std::string ByteReader::readUTF8String(int length) {
    std::stringstream ss;
    if (length == 0) {
        return ss.str();
    }
    for (auto i = 0; i < length; i++) {
        auto c = stream.get();
        switch (c >> 4) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7: {
                ss << static_cast<char>(c);
                break;
            }
            case 12:
            case 13: {
                ss << static_cast<char>(c);
                ss << static_cast<char>(stream.get());
                break;
            }
            case 14: {
                ss << static_cast<char>(c);
                ss << static_cast<char>(stream.get());
                ss << static_cast<char>(stream.get());
                break;
            }
            case 15: {
                if ((c & 8) == 8) {
                    throw std::runtime_error("bad utf-8 encode");
                }
                ss << static_cast<char>(c);
                ss << static_cast<char>(stream.get());
                ss << static_cast<char>(stream.get());
                ss << static_cast<char>(stream.get());
                i++;
                break;
            }
            default:
                throw std::runtime_error("bad utf-8 encode");
        };
    }
    return ss.str();
}

}
} // hprose::io
