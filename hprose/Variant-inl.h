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

} // hprose
