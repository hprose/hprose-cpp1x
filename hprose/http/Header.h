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
 * hprose/http/Header.h                                   *
 *                                                        *
 * hprose http header for cpp.                            *
 *                                                        *
 * LastModified: Dec 12, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace hprose {
namespace http {

class Header
    : public std::unordered_map<std::string, std::vector<std::string> > {
public:
    void add(std::string key, std::string value) {
        auto search = find(key);
        if (search != end()) {
            search->second.push_back(value);
        } else {
            set(key, value);
        }
    }

    inline void set(std::string key, std::string value) {
        (*this)[key] = {value};
    }

    std::string get(std::string key) {
        auto search = find(key);
        if (search != end()) {
            auto value = search->second;
            if (value.size()) {
                return value[0];
            }
        }
        return "";
    }

    inline void del(std::string key) {
        erase(key);
    }

};

}
} // hprose::http
