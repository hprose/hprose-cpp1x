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

    void setName(const std::string &name) { this->name = name; };

    std::string getValue() const { return value; }

    void setValue(const std::string &value) { this->value = value; };

    std::string getDomain() const { return domain; }

    void setDomain(const std::string &domain) { this->domain = domain; };

    std::string getPath() const { return path; }

    void setPath(const std::string &path) { this->path = path; };

    std::time_t getExpires() const { return expires; }

    void setExpires(std::time_t expires) { this->expires = expires; };

    int getMaxAge() const { return maxAge; }

    void setMaxAge(int maxAge) { this->maxAge = maxAge; };

    bool isSecure() const { return secure; }

    void setSecure(bool secure) { this->secure = secure; };

    bool isHttpOnly() const { return httpOnly; }

    void setHttpOnly(bool httpOnly) { this->httpOnly = httpOnly; };

    std::string str() const;

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
