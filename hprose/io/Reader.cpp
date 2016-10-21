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
 * LastModified: Oct 21, 2016                             *
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
        case tags::TagInteger:
            return "int";
        case tags::TagLong:
            return "long long";
        case tags::TagDouble:
            return "double";
        case tags::TagNull:
            return "null";
        case tags::TagEmpty:
            return "empty string";
        case tags::TagTrue:
            return "true";
        case tags::TagFalse:
            return "false";
        case tags::TagNaN:
            return "nan";
        case tags::TagInfinity:
            return "infinity";
        case tags::TagDate:
        case tags::TagTime:
            return "time";
        case tags::TagBytes:
            return "bytes";
        case tags::TagUTF8Char:
        case tags::TagString:
            return "string";
        case tags::TagGuid:
            return "uuid";
        case tags::TagList:
            return "list";
        case tags::TagMap:
            return "map";
        case tags::TagClass:
            return "class";
        case tags::TagObject:
            return "object";
        case tags::TagRef:
            return "reference";
        default:
            throw UnexpectedTag(tag);
    }

}

}  // internal

int64_t Reader::readInt64(char tag) {
    auto b = stream.get();
    if (b == tag) {
        return 0;
    }
    auto i = 0;
    auto neg = false;
    switch (b) {
        case '-':
            neg = true;
        case '+':
            b = stream.get();
        default:
            break;
    }
    if (neg) {
        while (b != tag) {
            i = i * 10 - (b - '0');
            b = stream.get();
        }
    } else {
        while
            (b != tag) {
            i = i * 10 + (b - '0');
            b = stream.get();
        }
    }
    return i;
}

}
} // hprose::io
