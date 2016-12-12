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
 * LastModified: Dec 12, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <ctime>
#include <string>

namespace hprose {
namespace http {

struct Cookie {
    Cookie()
        : expiry(0), secure(false), httpOnly(false) {
    }

    inline bool expired() const {
        return expiry && (expiry < time(0));
    };

    inline bool good() const {
        return !value.empty();
    };

    inline std::string raw() const {
        return name + "=" + value;
    };

    std::string name;
    std::string value;
    std::string domain;
    std::string path;
    std::time_t expiry;
    bool secure;
    bool httpOnly;
};

}
} // hprose::http
