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
 * hprose/http/Cookie.h                                   *
 *                                                        *
 * hprose http cookie for cpp.                            *
 *                                                        *
 * LastModified: Dec 3, 2017                              *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#include <hprose/http/Cookie.h>

#include <sstream>

namespace hprose {
namespace http {

bool isCookieNameValid(const std::string &name) {
    if (name.empty()) {
        return false;
    }
    return true;
}


std::string Cookie::str() const {
    if (!isCookieNameValid(name)) {
        return "";
    }
    std::ostringstream stream;
    stream << name << "=" << value;
    return stream.str();
}

}
} // hprose::http
