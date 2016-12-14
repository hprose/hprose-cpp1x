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
 * LastModified: Dec 14, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <algorithm>
#include <locale>
#include <string>
#include <vector>
#include <unordered_map>

namespace hprose {
namespace http {

namespace internal {

struct UniHash {
    size_t operator()(const std::string &str) const {
        std::string lowerStr(str.size(), 0);
        std::transform(str.begin(), str.end(), lowerStr.begin(), tolower);
        std::hash<std::string> hash;
        return hash(lowerStr);
    }
};

struct UniEqual {
    bool operator()(const std::string &left, const std::string &right) const {
        return left.size() == right.size()
               && std::equal(left.begin(), left.end(), right.begin(),
                             [](char a, char b) {
                                 return tolower(a) == tolower(b);
                             }
        );
    }
};

} // internal

class Header
    : public std::unordered_map<std::string, std::vector<std::string>, internal::UniHash, internal::UniEqual> {
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
