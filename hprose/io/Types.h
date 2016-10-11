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
 * hprose/io/Types.h                                      *
 *                                                        *
 * hprose types header for cpp.                           *
 *                                                        *
 * LastModified: Oct 11, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <type_traits>

namespace hprose { namespace io { 

template<int v>
struct IntToType {
    const static int value = v;
};

typedef IntToType<-4> UnknownType;
typedef IntToType< 1> BoolType;
typedef IntToType< 2> ByteType;
typedef IntToType< 3> CharType;
typedef IntToType< 4> EnumType;
typedef IntToType< 5> IntegerType;

template<typename Type>
struct TypeToType {
    typedef IntToType<UnknownType::value> type;
};

template<>
struct TypeToType<bool> {
    typedef BoolType type;
};

template<typename Type>
struct NonCVType
    : public TypeToType<typename std::remove_cv<Type>::type>::type {
};

} } // hprose::io
