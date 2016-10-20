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
 * LastModified: Oct 20, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <complex>
#include <ratio>
#include <string>
#include <array>
#include <vector>
#include <deque>
#include <forward_list>
#include <list>
#include <set>
#include <unordered_set>
#include <bitset>
#include <tuple>
#include <map>
#include <unordered_map>
#include <type_traits>

namespace hprose {
namespace io {

template<int v>
struct IntToType {
    const static int value = v;
};

typedef IntToType<-4> UnknownType;
typedef IntToType<-3> NullPtrType;
typedef IntToType< 1> BoolType;
typedef IntToType< 2> ByteType;
typedef IntToType< 3> CharType;
typedef IntToType< 4> EnumType;
typedef IntToType< 5> IntegerType;
typedef IntToType< 6> FloatType;
typedef IntToType< 7> ComplexType;
typedef IntToType< 8> RatioType;
typedef IntToType< 9> TimeType;
typedef IntToType<10> StringType;
typedef IntToType<11> UUIDType;
typedef IntToType<12> ListType;
typedef IntToType<13> MapType;
typedef IntToType<14> ObjectType;

template<typename Type>
struct TypeToType {
    typedef IntToType<UnknownType::value> type;
};

template<>
struct TypeToType<std::nullptr_t> {
    typedef NullPtrType type;
};

template<>
struct TypeToType<bool> {
    typedef BoolType type;
};

template<>
struct TypeToType<signed char> {
    typedef IntegerType type;
};

template<>
struct TypeToType<unsigned char> {
    typedef IntegerType type;
};

template<>
struct TypeToType<short> {
    typedef IntegerType type;
};

template<>
struct TypeToType<unsigned short> {
    typedef IntegerType type;
};

template<>
struct TypeToType<int> {
    typedef IntegerType type;
};

template<>
struct TypeToType<unsigned int> {
    typedef IntegerType type;
};

template<>
struct TypeToType<long> {
    typedef IntegerType type;
};

template<>
struct TypeToType<unsigned long> {
    typedef IntegerType type;
};

template<>
struct TypeToType<long long> {
    typedef IntegerType type;
};

template<>
struct TypeToType<unsigned long long> {
    typedef IntegerType type;
};

template<>
struct TypeToType<float> {
    typedef FloatType type;
};

template<>
struct TypeToType<double> {
    typedef FloatType type;
};

template<>
struct TypeToType<long double> {
    typedef FloatType type;
};

template<typename T>
struct TypeToType<std::complex<T> > {
    typedef ComplexType type;
};

template<intmax_t N, intmax_t D>
struct TypeToType<std::ratio<N, D> > {
    typedef RatioType type;
};

template<typename Element, typename Traits, typename Allocator>
struct TypeToType<std::basic_string<Element, Traits, Allocator> > {
    typedef StringType type;
};

template<typename T, size_t N>
struct TypeToType<std::array<T, N> > {
    typedef ListType type;
};

template<typename T, typename Allocator>
struct TypeToType<std::vector<T, Allocator> > {
    typedef ListType type;
};

template<typename T, typename Allocator>
struct TypeToType<std::deque<T, Allocator> > {
    typedef ListType type;
};

template<typename T, typename Allocator>
struct TypeToType<std::forward_list<T, Allocator> > {
    typedef ListType type;
};

template<typename T, typename Allocator>
struct TypeToType<std::list<T, Allocator> > {
    typedef ListType type;
};

template<typename Key, typename Compare, typename Allocator>
struct TypeToType<std::set<Key, Compare, Allocator> > {
    typedef ListType type;
};

template<typename Key, typename Compare, typename Allocator>
struct TypeToType<std::multiset<Key, Compare, Allocator> > {
    typedef ListType type;
};

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
struct TypeToType<std::unordered_set<Key, Hash, KeyEqual, Allocator> > {
    typedef ListType type;
};

template<typename Key, typename Hash, typename KeyEqual, typename Allocator>
struct TypeToType<std::unordered_multiset<Key, Hash, KeyEqual, Allocator> > {
    typedef ListType type;
};

template<size_t N>
struct TypeToType<std::bitset<N> > {
    typedef ListType type;
};

template<typename... Types>
struct TypeToType<std::tuple<Types...> > {
    typedef ListType type;
};

template<typename Key, typename T, typename Compare, typename Allocator>
struct TypeToType<std::map<Key, T, Compare, Allocator> > {
    typedef MapType type;
};

template<typename Key, typename T, typename Compare, typename Allocator>
struct TypeToType<std::multimap<Key, T, Compare, Allocator> > {
    typedef MapType type;
};

template<typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
struct TypeToType<std::unordered_map<Key, T, Hash, KeyEqual, Allocator> > {
    typedef MapType type;
};

template<typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
struct TypeToType<std::unordered_multimap<Key, T, Hash, KeyEqual, Allocator> > {
    typedef MapType type;
};

template<typename Type>
struct NonCVType
    : public TypeToType<typename std::remove_cv<Type>::type>::type {
};

}
} // hprose::io
