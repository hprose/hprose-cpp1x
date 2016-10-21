#include <hprose/io/decoders/BoolDecoder.h>
#include <hprose/io/Reader.h>

namespace hprose {
namespace io {
namespace decoders {

bool readNumberAsBool(Reader &reader) {
    std::string s = reader.readUntil(tags::TagSemicolon);
    if (s.length() == 1) {
        return s[0] != '0';
    } else {
        return true;
    }
}

bool readInfAsBool(Reader &reader) {
    reader.readInfinity<float>();
    return true;
}

bool readUTF8CharAsBool(Reader &reader) {
    return false;
}

bool readStringAsBool(Reader &reader) {
    return false;
}

bool BoolDecode(Reader &reader, char tag) {
    switch (tag) {
        case '0':
        case tags::TagNull:
        case tags::TagEmpty:
        case tags::TagFalse:
            return false;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case tags::TagTrue:
        case tags::TagNaN:
            return true;
        case tags::TagInteger:
        case tags::TagLong:
        case tags::TagDouble:
            return readNumberAsBool(reader);
        case tags::TagInfinity:
            return readInfAsBool(reader);
        case tags::TagUTF8Char:
            return readUTF8CharAsBool(reader);
        case tags::TagString:
            return readStringAsBool(reader);
        default:
            return false;
    }
}

}
}
} // hprose::io::decoders
