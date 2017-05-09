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
 * hprose/Uri.h                                           *
 *                                                        *
 * URI for cpp.                                           *
 *                                                        *
 * LastModified: Dec 9, 2016                              *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <string>

namespace hprose {

class Uri {
public:
    explicit Uri(const std::string &str);
    Uri();
    ~Uri();

    void setUri(const std::string &str);

    const std::string &getScheme() const { return scheme; }

    const std::string &getUsername() const { return username; }

    const std::string &getPassword() const { return password; }

    const std::string &getHost() const { return host; }

    uint16_t getPort() const { return port; }

    void setPort(uint16_t port) { this->port = port; };

    const std::string &getPath() const { return path; }

    const std::string &getQuery() const { return query; }

    const std::string &getFragment() const { return fragment; }

    std::string getHostname() const;

    std::string getAuthority() const;

    std::string str() const;

private:
    std::string scheme;
    std::string username;
    std::string password;
    std::string host;
    uint16_t port;
    std::string path;
    std::string query;
    std::string fragment;
};

} // hprose
