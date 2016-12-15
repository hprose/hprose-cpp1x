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
 * hprose/io/RawReader.h                                  *
 *                                                        *
 * hprose raw reader for cpp.                             *
 *                                                        *
 * LastModified: Dec 15, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <hprose/io/ByteReader.h>

#include <sstream>

namespace hprose {
namespace io {

struct UnexpectedTag : std::runtime_error {
    explicit UnexpectedTag(char tag)
        : std::runtime_error(tag == -1 ?
                             "no byte found in stream" :
                             std::string("unexpected serialize tag '") + tag + "' in stream") {
    }
};

class RawReader : public ByteReader {
public:
    RawReader(std::istream &stream)
        : ByteReader(stream) {
    }

    inline std::string readRaw() {
        std::ostringstream stream;
        readRawTo(stream);
        return stream.str();
    }

    inline void readRawTo(std::ostream &ostream) {
        readRaw(ostream, static_cast<char>(stream.get()));
    }

private:
    void readRaw(std::ostream &ostream, char tag);

    void readNumberRaw(std::ostream &ostream);

    void readDateTimeRaw(std::ostream &ostream);

    void readUTF8CharRaw(std::ostream &ostream);

    void readStringRaw(std::ostream &ostream);

    void readBytesRaw(std::ostream &ostream);

    void readGUIDRaw(std::ostream &ostream);

    void readComplexRaw(std::ostream &ostream);
};

}
} // hprose::io
