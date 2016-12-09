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
 * hprose/Uri.cpp                                         *
 *                                                        *
 * URI for cpp.                                           *
 *                                                        *
 * LastModified: Dec 9, 2016                              *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#include <hprose/Uri.h>

namespace hprose {

Uri::Uri(const std::string &str) {
    std::string surl;
    std::string::size_type x, y;
    scheme = "http";
    username = "";
    password = "";
    port = 80;
    x = str.find("://");
    if (x != std::string::npos) {
        scheme = str.substr(0, x);
        surl = str.substr(x + 3);
    } else {
        surl = str;
    }
    std::transform(scheme.begin(), scheme.end(), scheme.begin(), tolower);
    if (scheme == "https") {
        port = 443;
    }
    x = surl.find('@');
    y = surl.find('/');
    if ((x != std::string::npos) && ((x < y) || (y == std::string::npos))) {
        username = surl.substr(0, x);
        surl.erase(0, x + 1);
        x = username.find(':');
        if (x != std::string::npos) {
            password = username.substr(x + 1);
            username.erase(x);
        }
    }
    x = surl.find('/');
    if (x != std::string::npos) {
        host = surl.substr(0, x);
        surl.erase(0, x + 1);
    } else {
        host = surl;
        surl = "";
    }
    bool ipv6 = host[0] == '[';
    if (ipv6) {
        x = host.find(']');
        if ((x + 1 < host.size()) && (host[x + 1] == ':')) {
            port = (uint16_t) std::stoi(host.substr(x + 2));
        }
        host = host.substr(1, x - 1);
    } else {
        x = host.find(':');
        if (x != std::string::npos) {
            port = (uint16_t) std::stoi(host.substr(x + 1));
            host.erase(x);
        }
    }
    x = surl.find('?');
    if (x != std::string::npos) {
        path = '/' + surl.substr(0, x);
    } else {
        path = '/' + surl;
    }
    if (host == "") {
        host = "localhost";
    }
}

std::string Uri::hostname() const {
    if (host.size() > 0 && host[0] == '[') {
        return host.substr(1, host.size() - 2);
    }
    return host;
}

std::string Uri::str() const {
    return std::string();
}

} // hprose
