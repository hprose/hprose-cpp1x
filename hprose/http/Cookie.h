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
 * LastModified: Nov 29, 2017                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <ctime>
#include <string>

namespace hprose {
namespace http {

class Cookie {
public:
    Cookie(std::string name, std::string value)
        : name(name), value(value), expires(0), maxAge(-1), secure(false), httpOnly(false) {
    }

    std::string getName() const { return name; }
    std::string getValue() const { return value; }
    std::string getDomain() const { return domain; }
    std::string getPath() const { return path; }
    std::time_t getExpires() const { return expires; }
    int getMaxAge() const { return maxAge; }
    bool isSecure() const { return secure; }
    bool isHttpOnly() const { return httpOnly; }

    void setName(const std::string &name) { this->name = name; };
    void setValue(const std::string &value) { this->value = value; };
    void setDomain(const std::string &domain) { this->domain = domain; };
    void setPath(const std::string &path) { this->path = path; };
    void setExpires(std::time_t expires) { this->expires = expires; };
    void setMaxAge(int maxAge) { this->maxAge = maxAge; };
    void setSecure(bool secure) { this->secure = secure; };
    void setHttpOnly(bool httpOnly) { this->httpOnly = httpOnly; };

private:
    std::string name;
    std::string value;
    std::string domain;
    std::string path;
    std::time_t expires;
    int maxAge;
    bool secure;
    bool httpOnly;
};

}
} // hprose::http
