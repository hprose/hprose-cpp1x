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
 * hprose/io/RawReader.cpp                                *
 *                                                        *
 * hprose raw reader for cpp.                             *
 *                                                        *
 * LastModified: Dec 15, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#include <hprose/io/RawReader.h>

namespace hprose {
namespace io {

void RawReader::readRaw(std::ostream &ostream, char tag) {
    ostream << tag;
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
        case TagNull:
        case TagEmpty:
        case TagFalse:
        case TagTrue:
        case TagNaN:      break;
        case TagInfinity: ostream << stream.get(); break;
        case TagInteger:
        case TagLong:
        case TagDouble:
        case TagRef:      readNumberRaw(ostream); break;
        case TagDate:
        case TagTime:     readDateTimeRaw(ostream); break;
        case TagUTF8Char: readUTF8CharRaw(ostream); break;
        case TagString:   readStringRaw(ostream); break;
        case TagBytes:    readBytesRaw(ostream); break;
        case TagGUID:     readGUIDRaw(ostream); break;
        case TagList:
        case TagMap:
        case TagObject:   readComplexRaw(ostream); break;
        case TagClass:    readComplexRaw(ostream); readRawTo(ostream); break;
        case TagError:    readRawTo(ostream); break;
        default:          throw UnexpectedTag(tag);
    }
}

void RawReader::readNumberRaw(std::ostream &ostream) {
    while (!stream.eof()) {
        auto tag = static_cast<char>(stream.get());
        ostream << tag;
        if (tag == TagSemicolon) break;
    }
}

void RawReader::readDateTimeRaw(std::ostream &ostream) {
    while (!stream.eof()) {
        auto tag = static_cast<char>(stream.get());
        ostream << tag;
        if (tag == TagSemicolon || tag == TagUTC) break;
    }
}

void RawReader::readUTF8CharRaw(std::ostream &ostream) {
    ostream << readUTF8String(1);
}

void RawReader::readStringRaw(std::ostream &ostream) {
    auto count = 0;
    char tag = '0';
    while (!stream.eof()) {
        count *= 10;
        count += (tag - '0');
        tag = static_cast<char>(stream.get());
        ostream << tag;
        if (tag == TagQuote) {
            ostream << readUTF8String(count + 1);
            break;
        }
    }
}

void RawReader::readBytesRaw(std::ostream &ostream) {
    auto count = 0;
    char tag = '0';
    while (!stream.eof()) {
        count *= 10;
        count += (tag - '0');
        tag = static_cast<char>(stream.get());
        ostream << tag;
        if (tag == TagQuote) {
            ostream << read(count + 1);
            break;
        }
    }
}

void RawReader::readGUIDRaw(std::ostream &ostream) {
    ostream << read(38);
}

void RawReader::readComplexRaw(std::ostream &ostream) {
    char tag = 0;
    while (!stream.eof()) {
        tag = static_cast<char>(stream.get());
        ostream << tag;
        if (tag == TagOpenbrace) break;
    }
    while (!stream.eof()) {
        tag = static_cast<char>(stream.get());
        if (tag == TagClosebrace) {
            ostream << tag;
            break;
        }
        readRaw(ostream, tag);
    }
}

}
} // hprose::io
