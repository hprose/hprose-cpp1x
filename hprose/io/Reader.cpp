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
            return "Integer";
        case tags::TagLong:
            return "Long";
        case tags::TagDouble:
            return "Double";
        case tags::TagNull:
            return "Null";
        case tags::TagEmpty:
            return "Empty String";
        case tags::TagTrue:
            return "True";
        case tags::TagFalse:
            return "False";
        case tags::TagNaN:
            return "NaN";
        case tags::TagInfinity:
            return "Infinity";
        case tags::TagDate:
        case tags::TagTime:
            return "DateTime";
        case tags::TagBytes:
            return "Bytes";
        case tags::TagUTF8Char:
        case tags::TagString:
            return "String";
        case tags::TagGuid:
            return "GUID";
        case tags::TagList:
            return "List";
        case tags::TagMap:
            return "Map";
        case tags::TagClass:
        case tags::TagObject:
            return "Object";
        case tags::TagRef:
            return "Reference";
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
