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

namespace hprose {

namespace detail {

struct Destroy {
    template<class T>
    static void destroy(T *t) { t->~T(); }
};

} // hprose::detail

inline Variant::Variant(std::string v) : type(String) {
    new (&data.string) std::shared_ptr<std::string>(new std::string(std::move(v)));
}

inline Variant::Variant(std::tm v) : type(Time) {
    new (&data.time) std::shared_ptr<std::tm>(new std::tm(std::move(v)));
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

inline bool Variant::isString() const {
    return type == String;
}

inline bool Variant::isTime() const {
    return type == Time;
}

inline const std::string &Variant::getString() const & {
    return *data.string;
}

inline const std::tm &Variant::getTime() const & {
    return *data.time;
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
    static void **get(Data &d) noexcept { return &d.null; }
};

template<>
struct Variant::GetAddrImpl<std::shared_ptr<std::string> > {
    static std::shared_ptr<std::string> *get(Data &d) noexcept { return &d.string; }
};

template<>
struct Variant::GetAddrImpl<std::shared_ptr<std::tm> > {
    static std::shared_ptr<std::tm> *get(Data &d) noexcept { return &d.time; }
};

} // hprose
