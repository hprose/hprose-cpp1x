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
 * hprose/Ref.h                                           *
 *                                                        *
 * ref type for cpp.                                      *
 *                                                        *
 * LastModified: Dec 26, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <typeinfo>
#include <functional>

namespace hprose {

struct Ref {
    template<class T>
    Ref(const T &v)
        : ptr(&v), type(&typeid(v)) {
    }

    const void *ptr;
    const std::type_info *type;
};

} // hprose

namespace std {

template<>
struct hash<hprose::Ref> {
    typedef hprose::Ref argument_type;
    typedef std::size_t result_type;

    result_type operator()(const argument_type &ref) const {
        const result_type h1(std::hash<const void *>{}(ref.ptr));
        const result_type h2(std::hash<const std::type_info *>{}(ref.type));
        return h1 ^ (h2 << 1);
    }
};

template<>
struct equal_to<hprose::Ref> {
    typedef bool result_type;
    typedef hprose::Ref first_argument_type;
    typedef hprose::Ref second_argument_type;

    result_type operator()(const first_argument_type &left, const second_argument_type &right) const {
        return left.ptr == right.ptr && left.type == right.type;
    }
};

} // std
