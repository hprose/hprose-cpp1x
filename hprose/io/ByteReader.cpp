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
 * hprose/io/ByteReader.h                                 *
 *                                                        *
 * hprose byte reader for cpp.                            *
 *                                                        *
 * LastModified: Dec 15, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#include <hprose/io/ByteReader.h>

#include <sstream>

namespace hprose {
namespace io {

std::string ByteReader::read(size_t count) {
    std::string str;
    str.resize(count);
    stream.read(const_cast<char *>(str.data()), count);
    if (static_cast<size_t>(stream.gcount()) != count) {
        throw std::runtime_error("unexpected end of stream");
    }
    return str;
}

std::string ByteReader::readUTF8String(int length) {
    if (length == 0) {
        return "";
    }
    std::ostringstream ss;
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
