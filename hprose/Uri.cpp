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
 * LastModified: Dec 15, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#include <hprose/Uri.h>
#include <hprose/util/Util.h>

#include <algorithm>
#ifdef HPROSE_HAS_REGEX
#include <regex>
#else // HPROSE_HAS_REGEX
#include <string>
#include <cstdlib>
#include <cstring>
#endif // HPROSE_HAS_REGEX
#include <sstream>

namespace hprose {

#ifdef HPROSE_HAS_REGEX
std::string submatch(const std::smatch &m, size_t idx) {
    auto &sub = m[idx];
    return std::string(sub.first, sub.second);
}

void Uri::setUri(const std::string &str) {
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
            throw std::invalid_argument(
                std::string("invalid URI authority ") + std::string(authority.first, authority.second));
        }

        const std::string port(authorityMatch[4].first, authorityMatch[4].second);
        if (!port.empty()) {
            this->port = static_cast<uint16_t>(std::stoi(port));
        }

        username = submatch(authorityMatch, 1);
        password = submatch(authorityMatch, 2);
        host = submatch(authorityMatch, 3);
        path = submatch(authorityAndPathMatch, 2);
    }

    query = submatch(match, 3);
    fragment = submatch(match, 4);
}

#else // HPROSE_HAS_REGEX

static std::string getLeftStr(std::string &str, const char sep) {
    std::string ret;
    std::string::size_type pos = str.find(sep);
    if (pos != std::string::npos) {
        ret = str.substr(0, pos);
        str.erase(0, pos + 1);
    }
    return ret;
}

static std::string getRightStr(std::string &str, const char sep) {
    std::string ret;
    std::string::size_type pos = str.find(sep);
    if (pos != std::string::npos) {
        ret = str.substr(pos + 1);
        str.erase(pos);
    }
    return ret;
}

void Uri::setUri(const std::string &str) {
    //scheme:[//[user[:password]@]host[:port]][/path][?query][#fragment]
    path = str;
    scheme = getLeftStr(path, ':');
    if (!scheme.empty()) {
        std::transform(scheme.begin(), scheme.end(), scheme.begin(), tolower);
    }

    fragment = getRightStr(path, '#');
    query = getRightStr(path, '?');

    std::string::size_type pos = path.find("//");
    if (pos != std::string::npos) {
        path.erase(0, 2);
        pos = path.find("/");
        host = path;
        path = getRightStr(host, '/');
        if (pos != std::string::npos) {
            path.insert(0, 1, '/');
        }
        if (!host.empty()) {
            username = getLeftStr(host, '@');
            if (!username.empty()) {
                password = getRightStr(username, ':');
            }
            std::string strPort = getRightStr(host, ':');
            if (!strPort.empty()) {
                port = static_cast<uint16_t>(std::stoi(strPort));
            }
        }
    }
}
#endif // HPROSE_HAS_REGEX

Uri::Uri(const std::string &str)
    : port(0) {
    setUri(str);
}

Uri::Uri()
    : port(0) {
}

Uri::~Uri() {
}

std::string Uri::getHostname() const {
    if (host.size() > 0 && host[0] == '[') {
        return host.substr(1, host.size() - 2);
    }
    return host;
}

std::string Uri::getAuthority() const {
    std::ostringstream ss;
    if (!username.empty() || !password.empty()) {
        ss << username;
        if (!password.empty()) {
            ss << ':' << password;
        }
        ss << '@';
    }
    ss << host;
    if (port != 0) {
        ss << ':' << port;
    }
    return ss.str();
}

std::string Uri::str() const {
    std::ostringstream ss;
    auto authority = getAuthority();
    if (!authority.empty()) {
        ss << scheme << "://" << authority;
    } else {
        ss << scheme << ':';
    }
    ss << path;
    if (!query.empty()) {
        ss << '?' << query;
    }
    if (!fragment.empty()) {
        ss << '#' << fragment;
    }
    return ss.str();
}

} // hprose
