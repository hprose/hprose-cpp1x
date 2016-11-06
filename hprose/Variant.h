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
 * hprose/Variant.h                                       *
 *                                                        *
 * variant type for cpp.                                  *
 *                                                        *
 * LastModified: Nov 6, 2016                              *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <cstdlib>
#include <ctime>
#include <string>
#include <memory>
#include <type_traits>

namespace hprose {

class Variant {
public:
    enum Type {
        Null,
        String,
        Time
    };

    Variant(std::string v);

    Variant(std::tm v);

    Variant(const Variant &o);
    Variant(Variant &&o) noexcept;
    ~Variant() noexcept;

    Variant &operator=(const Variant &o);
    Variant &operator=(Variant &&o) noexcept;

    inline bool isString() const {
        return type == String;
    }

    const char *typeName() const;

    const std::string &getString() const &{
        return *data.string;
    }

private:

    void destroy() noexcept;

    Type type;

    union Data {
        explicit Data() : nul(nullptr) {}

        ~Data() {}

        void *nul;
        std::shared_ptr<std::string> string;
        std::shared_ptr<std::tm> time;
    } data;
};

} // hprose

#include <hprose/Variant-inl.h>
