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
 * hprose/util/Config.h                                   *
 *                                                        *
 * some compiler config for cpp.                          *
 *                                                        *
 * LastModified: May 22, 2017                             *
 * Author: xiaoyur347 <xiaoyur347@gmail.com>              *
 *                                                        *
\**********************************************************/

#pragma once

// compiler macros
#if defined(__GNUC__)
    #include <cstddef> // for _GLIBCXX_USE_C99
    #if defined(__clang__) || __GNUC__ > 5 || defined(_GLIBCXX_USE_C99)
        #define HPROSE_HAS_STOX
    #endif
#else
    #define HPROSE_HAS_STOX
#endif

#if defined(__GNUC__)
    #if defined(__clang__) || __GNUC__ > 4
        #define HPROSE_HAS_CODECVT
    #endif
#elif defined(_MSC_VER)
    #define HPROSE_HAS_CODECVT
    //this is a bug of Visual Studio
    //https://social.msdn.microsoft.com/Forums/en-US/8f40dcd8-c67f-4eba-9134-a19b9178e481/vs-2015-rc-linker-stdcodecvt-error?forum=vcgeneral
    #define HPROSE_HAS_CODECVT_BUG
#else
    #define HPROSE_HAS_CODECVT
#endif

#if defined(__GNUC__)
    #if defined(__clang__) || __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 8)
        #define HPROSE_HAS_REGEX
    #endif
#else
    #define HPROSE_HAS_REGEX
#endif

#if defined(__GNUC__)
    #if defined(__clang__) || __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 7)
        #define HPROSE_HAS_REF_QUALIFIER
    #endif
#else
    #define HPROSE_HAS_REF_QUALIFIER
#endif

#if defined(__GNUC__)
    #if defined(__clang__) || __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 7)
        #define HPROSE_HAS_ARRAY_INITIALIZER_LIST
    #endif
#elif defined(_MSC_VER)
    //#define HPROSE_HAS_ARRAY_INITIALIZER_LIST
#else
    #define HPROSE_HAS_ARRAY_INITIALIZER_LIST
#endif

#if defined(__GNUC__)
    #if defined(__clang__) || __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 6)
        #define HPROSE_HAS_DELEGATING_CONSTRUCTORS
    #endif
#else
    #define HPROSE_HAS_DELEGATING_CONSTRUCTORS
#endif

#if defined(__GNUC__)
    #if defined(__clang__) || __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 6)
        #define HPROSE_HAS_UNDERLYING_TYPE
    #endif
#else
    #define HPROSE_HAS_UNDERLYING_TYPE
#endif

#if defined(__GNUC__)
    #if defined(__clang__) || __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 6)
        #define HPROSE_HAS_LAMBDA_CAPTURE
    #endif
#else
    #define HPROSE_HAS_LAMBDA_CAPTURE
#endif