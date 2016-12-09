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

#include <algorithm>
#include <regex>

namespace hprose {

std::string submatch(const std::smatch &m, size_t idx) {
    auto &sub = m[idx];
    return std::string(sub.first, sub.second);
}

Uri::Uri(const std::string &str) : port(-1) {
    static const std::regex uriRegex(
        "([a-zA-Z][a-zA-Z0-9+.-]*):"  // scheme:
        "([^?#]*)"                    // authority and path
        "(?:\\?([^#]*))?"             // ?query
        "(?:#(.*))?");                // #fragment

    static const std::regex authorityAndPathRegex("//([^/]*)(/.*)?");

    std::smatch match;
    if (!std::regex_match(str.begin(), str.end(), match, uriRegex)) {
        throw std::invalid_argument(std::string("invalid URI ") + str);
    }

    scheme = submatch(match, 1);
    std::transform(scheme.begin(), scheme.end(), scheme.begin(), tolower);

    const std::string authorityAndPath(match[2].first, match[2].second);
    std::smatch authorityAndPathMatch;
    if (!std::regex_match(authorityAndPath.begin(),
                          authorityAndPath.end(),
                          authorityAndPathMatch,
                          authorityAndPathRegex)) {
        // Does not start with //, doesn't have authority
        path = authorityAndPath;
    } else {
        static const std::regex authorityRegex(
            "(?:([^@:]*)(?::([^@]*))?@)?"  // username, password
            "(\\[[^\\]]*\\]|[^\\[:]*)"     // host (IP-literal (e.g. '['+IPv6+']',
            // dotted-IPv4, or named host)
            "(?::(\\d*))?");               // port

        auto authority = authorityAndPathMatch[1];
        std::smatch authorityMatch;
        if (!std::regex_match(authority.first,
                              authority.second,
                              authorityMatch,
                              authorityRegex)) {
            throw std::invalid_argument(std::string("invalid URI authority ") + std::string(authority.first, authority.second));
        }

        const std::string port(authorityMatch[4].first, authorityMatch[4].second);
        if (!port.empty()) {
            this->port = std::stoi(port);
        }

        username = submatch(authorityMatch, 1);
        password = submatch(authorityMatch, 2);
        host = submatch(authorityMatch, 3);
        path = submatch(authorityAndPathMatch, 2);
    }

    query = submatch(match, 3);
    fragment = submatch(match, 4);
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
