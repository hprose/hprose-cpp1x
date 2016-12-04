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

namespace hprose {

namespace detail {

struct Destroy {
    template<class T>
    static void destroy(T *t) { t->~T(); }
};

} // hprose::detail

inline Variant::Variant() : Variant(nullptr) {}

inline Variant::Variant(std::nullptr_t) : type(Null) {}

inline Variant::Variant(const char *v) : Variant(std::string(v)) {}

inline Variant::Variant(std::string v) : type(String) {
    new (&data.vString) std::shared_ptr<std::string>(new std::string(std::move(v)));
}

inline Variant::Variant(std::tm v) : type(Time) {
    new (&data.vTime) std::shared_ptr<std::tm>(new std::tm(std::move(v)));
}

template<class T>
Variant::Variant(const T &v) : type(Other) {
    new (&data.vOther) std::shared_ptr<Any>(new Any(v));
}

inline Variant::Variant(const Variant &o) : type(Null) {
    *this = o;
}

inline Variant::Variant(Variant &&o) noexcept : type(Null) {
    *this = std::move(o);
}

inline Variant::~Variant() noexcept {
    destroy();
}

inline bool Variant::isNull() const { return type == Null; }
inline bool Variant::isBool() const { return type == Bool; }
inline bool Variant::isInt64() const { return type == Int64; }
inline bool Variant::isDouble() const { return type == Double; }
inline bool Variant::isString() const { return type == String; }
inline bool Variant::isTime() const { return type == Time; }

inline const std::string &Variant::getString() const & {
    return *data.vString;
}

inline const std::tm &Variant::getTime() const & {
    return *data.vTime;
}

template<class T>
T *Variant::getAddress() noexcept {
    return GetAddrImpl<T>::get(data);
}

template<class T>
T const *Variant::getAddress() const noexcept {
    return const_cast<Variant *>(this)->getAddress<T>();
}

template<class T>
struct Variant::GetAddrImpl {
};

template<>
struct Variant::GetAddrImpl<void *> {
    static void **get(Data &d) noexcept { return &d.vNull; }
};

template<>
struct Variant::GetAddrImpl<std::shared_ptr<std::string> > {
    static std::shared_ptr<std::string> *get(Data &d) noexcept { return &d.vString; }
};

template<>
struct Variant::GetAddrImpl<std::shared_ptr<std::tm> > {
    static std::shared_ptr<std::tm> *get(Data &d) noexcept { return &d.vTime; }
};

template<>
struct Variant::GetAddrImpl<std::shared_ptr<Any> > {
    static std::shared_ptr<Any> *get(Data &d) noexcept { return &d.vOther; }
};

} // hprose
