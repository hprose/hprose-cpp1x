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
 * LastModified: Dec 4, 2016                              *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <hprose/Any.h>

#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <memory>
#include <type_traits>
#include <typeindex>

namespace hprose {

class Variant {
public:
    enum Type {
        Null,
        Bool,
        Int64,
        Double,
        String,
        Bytes,
        Time,
        Array,
        Other
    };

    Variant();
    Variant(std::nullptr_t);

    Variant(const char *v);
    Variant(std::string v);

    Variant(std::tm v);

    template<class T>
    Variant(const T &v);

    Variant(const Variant &o);
    Variant(Variant &&o) noexcept;
    ~Variant() noexcept;

    Variant &operator=(const Variant &o);
    Variant &operator=(Variant &&o) noexcept;

    bool isNull() const;
    bool isBool() const;
    bool isInt64() const;
    bool isDouble() const;
    bool isString() const;
    bool isTime() const;

    const char *typeName() const;

    const std::string &getString() const &;
    const std::tm &getTime() const &;

private:
    template<class T> struct GetAddrImpl;

    template<class T> T       *getAddress() noexcept;
    template<class T> T const *getAddress() const noexcept;

    void destroy() noexcept;

    Type type;

    union Data {
        explicit Data() : vNull(nullptr) {}
        ~Data() {}

        void *vNull;
        bool vBool;
        int64_t vInt64;
        double vDouble;
        std::shared_ptr<std::string> vString;
        std::shared_ptr<std::tm> vTime;
        std::shared_ptr<std::vector<Variant> > vArray;
        std::shared_ptr<Any> vOther;
    } data;
};

} // hprose

#include <hprose/Variant-inl.h>
