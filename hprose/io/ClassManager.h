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
 * hprose/io/ClassManager.h                               *
 *                                                        *
 * hprose class manager for cpp.                          *
 *                                                        *
 * LastModified: Dec 18, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <string>
#include <functional>
#include <vector>
#include <unordered_map>
#include <typeindex>

#define HPROSE_PP_CAT(a, b) HPROSE_PP_CAT_I(a, b)
#define HPROSE_PP_CAT_I(a, b) a ## b

#ifndef _MSC_VER
#define HPROSE_PP_VARIADIC_SIZE(...) HPROSE_PP_VARIADIC_SIZE_I(__VA_ARGS__, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,)
#else // _MSC_VER
//http://stackoverflow.com/questions/5530505/why-does-this-variadic-argument-count-macro-fail-with-vc
#define EXPAND(x) x
#define HPROSE_PP_VARIADIC_SIZE(...) EXPAND(HPROSE_PP_VARIADIC_SIZE_I(__VA_ARGS__, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,))
#endif // _MSC_VER
#define HPROSE_PP_VARIADIC_SIZE_I(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63, size, ...) size

#define HPROSE_PP_OVERLOAD(prefix, ...) HPROSE_PP_CAT(prefix, HPROSE_PP_VARIADIC_SIZE(__VA_ARGS__))

#define HPROSE_REG_CLASS_1(Class, Body) HPROSE_REG_CLASS_2(Class, #Class, Body)
#define HPROSE_REG_CLASS_2(Class, Alias, Body)                     \
namespace hprose { namespace io {                                  \
                                                                   \
template<>                                                         \
std::vector<FieldCache> getClassFields<Class>() {                  \
    typedef const Class * ConstClassPointer;                       \
    typedef Class * ClassPointer;                                  \
    std::vector<FieldCache> fields;                                \
    Body                                                           \
    return fields;                                                 \
}                                                                  \
                                                                   \
template<>                                                         \
void initClassCache<Class>(ClassCache &classCache) {               \
    auto fields = getClassFields<Class>();                         \
    auto count = fields.size();                                    \
    std::ostringstream stream;                                     \
    stream << TagClass;                                            \
    util::WriteInt(stream, util::UTF16Length(Alias));              \
    stream << TagQuote << Alias << TagQuote;                       \
    if (count > 0) util::WriteInt(stream, count);                  \
    stream << TagOpenbrace;                                        \
    for (auto &&field : fields) {                                  \
        classCache.fieldMap[field.alias] = field;                  \
        stream << TagString;                                       \
        util::WriteInt(stream, util::UTF16Length(field.alias));    \
        stream << TagQuote << field.alias << TagQuote;             \
    }                                                              \
    stream << TagClosebrace;                                       \
    classCache.alias = Alias;                                      \
    classCache.fields = std::move(fields);                         \
    classCache.data = stream.str();                                \
}                                                                  \
                                                                   \
inline void encode(const Class &v, Writer &writer) {               \
    writer.writeObject(v);                                         \
}                                                                  \
                                                                   \
inline void decode(Class &v, Reader &reader) {                     \
    reader.readObject(v);                                          \
}                                                                  \
                                                                   \
} } // hprose::io

#define HPROSE_REG_FIELD_1(Field) HPROSE_REG_FIELD_2(Field, #Field)
#define HPROSE_REG_FIELD_2(Field, Alias) {                             \
    FieldCache fieldCache;                                             \
    fieldCache.alias = Alias;                                          \
    fieldCache.encode = [](const void *obj, Writer &writer) {          \
        writer.writeValue(static_cast<ConstClassPointer>(obj)->Field); \
    };                                                                 \
    fieldCache.decode = [](void *obj, Reader &reader) {                \
        reader.readValue(static_cast<ClassPointer>(obj)->Field);       \
    };                                                                 \
    fields.push_back(std::move(fieldCache));                           \
}

#ifndef _MSC_VER
#define HPROSE_REG_CLASS(Class, ...) HPROSE_PP_OVERLOAD(HPROSE_REG_CLASS_,__VA_ARGS__)(Class, __VA_ARGS__)
#define HPROSE_REG_FIELD(...) HPROSE_PP_OVERLOAD(HPROSE_REG_FIELD_, __VA_ARGS__)(__VA_ARGS__)
#else // _MSC_VER
#define HPROSE_REG_CLASS(Class, ...) EXPAND(HPROSE_PP_OVERLOAD(HPROSE_REG_CLASS_,__VA_ARGS__)(Class, __VA_ARGS__))
#define HPROSE_REG_FIELD(...) EXPAND(HPROSE_PP_OVERLOAD(HPROSE_REG_FIELD_, __VA_ARGS__)(__VA_ARGS__))
#endif // _MSC_VER

namespace hprose {
namespace io {

class Writer;

class Reader;

struct FieldCache {
    std::string alias;
    std::function<void(const void *, Writer &)> encode;
    std::function<void(void *, Reader &)> decode;
};

struct ClassCache {
    std::string alias;
    std::vector<FieldCache> fields;
    std::unordered_map<std::string, FieldCache> fieldMap;
    std::string data;
};

template<class T>
std::vector<FieldCache> getClassFields();

template<class T>
void initClassCache(ClassCache &classCache);

class ClassManager {
public:
    static ClassManager &SharedInstance() {
        static ClassManager classManager;
        return classManager;
    }

    template<class T>
    void registerClass() {
        auto type = std::type_index(typeid(T));
        ClassCache classCache;
        initClassCache<T>(classCache);
        cache[type] = classCache;
        types[classCache.alias] = &typeid(T);
    }

    template<class T>
    const ClassCache &getClassCache() {
        auto type = std::type_index(typeid(T));
        auto iter = cache.find(type);
        if (iter == cache.end()) {
            registerClass<T>();
            return cache[type];
        } else {
            return iter->second;
        }
    }

    inline const ClassCache &getClassCache(const std::type_info &type) {
        return cache[std::type_index(type)];
    }

    inline const std::type_info *getClassType(const std::string &alias) {
        return types[alias];
    }

private:
    std::unordered_map<std::type_index, ClassCache> cache;
    std::unordered_map<std::string, const std::type_info *> types;
};

}
} // hprose::io
