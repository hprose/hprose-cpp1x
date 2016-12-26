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
