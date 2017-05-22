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
#include <hprose/util/PreProcessor.h>

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
#define HPROSE_REG_CLASS(Class, ...) HPROSE_PP_CAT(HPROSE_PP_OVERLOAD(HPROSE_REG_CLASS_,__VA_ARGS__)(Class, __VA_ARGS__),)
#define HPROSE_REG_FIELD(...) HPROSE_PP_CAT(HPROSE_PP_OVERLOAD(HPROSE_REG_FIELD_, __VA_ARGS__)(__VA_ARGS__),)
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
