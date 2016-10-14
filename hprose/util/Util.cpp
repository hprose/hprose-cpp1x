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
 * hprose/util/Util.cpp                                   *
 *                                                        *
 * some util for cpp.                                     *
 *                                                        *
 * LastModified: Oct 14, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#include <hprose/util/Util.h>

namespace hprose {
namespace util {

int UTF16Length(const std::string &str) {
    int length = str.length();
    int n = length;
    int p = 0;
    while (p < length) {
        char a = str.at(p);
        switch (static_cast<uint8_t>(a) >> 4) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
                p++;
                break;
            case 12:
            case 13:
                p += 2;
                n--;
                break;
            case 14:
                p += 3;
                n -= 2;
                break;
            case 15:
                if ((a & 8) == 8) {
                    return -1;
                }
                p += 4;
                n -= 2;
                break;
            default:
                return -1;
        }
    }
    return n;
}

}
} // hprose::util
