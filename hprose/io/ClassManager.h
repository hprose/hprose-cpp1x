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
 * LastModified: Nov 6, 2016                              *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <string>
#include <unordered_map>
#include <typeindex>

#define HPROSE_REGISTER(Class, ...)                          \
namespace hprose { namespace io {                            \
                                                             \
template<>                                                   \
std::string getClassCacheData<Class>() {                     \
    std::stringstream stream;                                \
    stream << tags::TagClass;                                \
    util::WriteInt(stream, util::UTF16Length(#Class));       \
    stream << tags::TagQuote << #Class << tags::TagQuote;    \
    stream << tags::TagOpenbrace;                            \
    stream << tags::TagClosebrace;                           \
    return stream.str();                                     \
}                                                            \
                                                             \
inline void encode(const Class &v, Writer &writer) {         \
    writer.writeObject(v);                                   \
}                                                            \
} } // hprose::io

namespace hprose {
namespace io {

struct ClassCache {
    std::string data;
};

template<class T>
std::string getClassCacheData();

class ClassManager {
public:
    static ClassManager &SharedInstance() {
        static ClassManager classManager;
        return classManager;
    }

    template<class T>
    const ClassCache &getClassCache() {
        auto type = std::type_index(typeid(T));
        auto iter = cache.find(type);
        if (iter == cache.end()) {
            ClassCache classCache;
            classCache.data = getClassCacheData<T>();
            cache[type] = classCache;
            return cache[type];
        } else {
            return iter->second;
        }
    }

private:
    std::unordered_map<std::type_index, ClassCache> cache;
};

}
} // hprose::io
