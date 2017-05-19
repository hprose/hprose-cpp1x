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
 * hprose/io/Writer.h                                     *
 *                                                        *
 * hprose writer header for cpp.                          *
 *                                                        *
 * LastModified: Dec 28, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <hprose/io/Tags.h>
#include <hprose/io/ClassManager.h>
#include <hprose/util/Util.h>
#include <hprose/Ref.h>

#include <ostream>
#include <memory>
#include <numeric>
#include <limits>
#include <complex>
#include <ratio>
#include <string>
#include <locale>
#ifdef HPROSE_HAS_CODECVT
#include <codecvt>
#endif // HPROSE_HAS_CODECVT
#include <chrono>
#include <array>
#include <vector>
#include <forward_list>
#include <tuple>
#include <bitset>
#include <unordered_map>
#include <type_traits>
#include <typeindex>

namespace hprose {
namespace io {

namespace internal {

class WriterRefer {
public:
    inline void addCount(size_t count) {
        lastref += count;
    }

    template<class T>
    inline void set(const T &v) {
        ref[Ref(v)] = lastref++;
    }

    template<class T>
    bool write(std::ostream &stream, const T &v) {
        auto r = ref.find(Ref(v));
        if (r != ref.end()) {
            stream << TagRef;
            util::WriteInt(stream, r->second);
            stream << TagSemicolon;
            return true;
        }
        return false;
    }

    inline void reset() {
        ref.clear();
        lastref = 0;
    }

private:
    std::unordered_map<Ref, int> ref;
    int lastref;
};

} // hprose::io::internal

class Writer {
public:
    Writer(std::ostream &stream, bool simple = false)
        : stream(stream), refer(simple ? nullptr : new internal::WriterRefer()) {
    }

    template<class T>
    inline Writer &serialize(const T &v) {
        writeValue(v);
        return *this;
    }

    template<class T>
    inline void writeValue(const T &v) {
        encode(v, *this);
    }

    inline void writeNull() {
        stream << TagNull;
    }

    inline void writeBool(bool b) {
        stream << (b ? TagTrue : TagFalse);
    }

    template<class T>
    typename std::enable_if<
        (std::is_integral<T>::value &&
         std::is_signed<T>::value &&
         sizeof(T) <= sizeof(int32_t))
    >::type
    writeInteger(T i) {
        if (i >= 0 && i <= 9) {
            stream << static_cast<char>('0' + i);
            return;
        }
        stream << TagInteger;
        util::WriteInt(stream, i);
        stream << TagSemicolon;
    }

    template<class T>
    typename std::enable_if<
        (std::is_integral<T>::value &&
         std::is_signed<T>::value &&
         sizeof(T) > sizeof(int32_t))
    >::type
    writeInteger(T i) {
        if (i >= 0 && i <= 9) {
            stream << static_cast<char>('0' + i);
            return;
        }
        if (i >= std::numeric_limits<int32_t>::min() && i <= std::numeric_limits<int32_t>::max()) {
            stream << TagInteger;
        } else {
            stream << TagLong;
        }
        util::WriteInt(stream, i);
        stream << TagSemicolon;
    }

    template<class T>
    typename std::enable_if<
        (std::is_integral<T>::value &&
         std::is_unsigned<T>::value &&
         sizeof(T) <= sizeof(uint16_t))
    >::type
    writeInteger(T u) {
        if (u <= 9) {
            stream << static_cast<char>('0' + u);
            return;
        }
        stream << TagInteger;
        util::WriteUint(stream, u);
        stream << TagSemicolon;
    }

    template<class T>
    typename std::enable_if<
        (std::is_integral<T>::value &&
         std::is_unsigned<T>::value &&
         sizeof(T) > sizeof(uint16_t))
    >::type
    writeInteger(T u) {
        if (u <= 9) {
            stream << static_cast<char>('0' + u);
            return;
        }
        if (u <= std::numeric_limits<int32_t>::max()) {
            stream << TagInteger;
        } else {
            stream << TagLong;
        }
        util::WriteUint(stream, u);
        stream << TagSemicolon;
    }

    template<class T>
    typename std::enable_if<
        std::is_floating_point<T>::value
    >::type
    writeFloat(T f) {
        if (f != f) {
            stream << TagNaN;
        } else if (f == std::numeric_limits<T>::infinity()) {
            stream << TagInfinity << TagPos;
        } else if (f == -std::numeric_limits<T>::infinity()) {
            stream << TagInfinity << TagNeg;
        } else {
            stream.precision(std::numeric_limits<T>::digits10);
            stream << TagDouble << f << TagSemicolon;
        }
    }

    template<class T>
    void writeComplex(const std::complex<T> &c) {
        if (c.imag() == 0) {
            writeFloat(c.real());
            return;
        }
        skipRef();
        writeListHeader(2);
        writeFloat(c.real());
        writeFloat(c.imag());
        writeListFooter();
    }

    template<intmax_t N, intmax_t D>
    void writeRatio(const std::ratio<N, D> &r) {
        if (r.den == 1) {
            writeInteger(r.num);
            return;
        }
        skipRef();
        std::string s = std::to_string(r.num) + "/" + std::to_string(r.den);
        writeString(s, s.length());
    }

    void writeString(const std::string &str) {
        int length = util::UTF16Length(str);
        if (length == 0) {
            stream << TagEmpty;
        } else if (length == 1) {
            stream << TagUTF8Char << str;
        } else if (length < 0) {
            writeBytes(reinterpret_cast<const uint8_t *>(str.data()), str.length());
        } else {
            if (writeRef(str)) return;
            setRef(str);
            writeString(str, length);
        }
    }

#ifdef HPROSE_HAS_CODECVT
    void writeString(const std::wstring &str) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        writeString(conv.to_bytes(str));
    }

    void writeString(const std::u16string &str) {
#ifndef HPROSE_HAS_CODECVT_BUG
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;
        writeString(conv.to_bytes(str));
#else // HPROSE_HAS_CODECVT_BUG
        std::wstring_convert<std::codecvt_utf8_utf16<uint16_t>, uint16_t> conv;
        writeString(conv.to_bytes(reinterpret_cast<const uint16_t *>(str.data())));
#endif // HPROSE_HAS_CODECVT_BUG
    }

    void writeString(const std::u32string &str) {
#ifndef HPROSE_HAS_CODECVT_BUG
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
        writeString(conv.to_bytes(str));
#else // HPROSE_HAS_CODECVT_BUG
        std::wstring_convert<std::codecvt_utf8<uint32_t>, uint32_t> conv;
        writeString(conv.to_bytes(reinterpret_cast<const uint32_t *>(str.data())));
#endif // HPROSE_HAS_CODECVT_BUG
    }
#endif // HPROSE_HAS_CODECVT

    void writeBytes(const uint8_t *data, size_t count) {
        skipRef();
        if (count == 0) {
            stream << TagBytes << TagQuote << TagQuote;
            return;
        }
        stream << TagBytes;
        util::WriteUint(stream, count);
        stream << TagQuote;
        stream.write(reinterpret_cast<const char *>(data), count);
        stream << TagQuote;
    }

    void writeTime(const std::tm &t) {
        if (writeRef(t)) return;
        setRef(t);
        if (t.tm_hour == 0 && t.tm_min == 0 && t.tm_sec == 0) {
            writeDate(1900 + t.tm_year, t.tm_mon + 1, t.tm_mday);
        } else if (t.tm_year == 70 && t.tm_mon == 0 && t.tm_mday == 1) {
            writeTime(t.tm_hour, t.tm_min, t.tm_sec, 0);
        } else {
            writeDate(1900 + t.tm_year, t.tm_mon + 1, t.tm_mday);
            writeTime(t.tm_hour, t.tm_min, t.tm_sec, 0);
        }
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
        stream << TagSemicolon;
#else
        stream << (t.tm_gmtoff == 0 ? TagUTC : TagSemicolon);
#endif
    }

    template<class Clock, class Duration>
    void writeTime(const std::chrono::time_point<Clock, Duration> &t) {
    }

    template<class T>
    void writeList(const T &lst) {
        if (writeRef(lst)) return;
        setRef(lst);
        size_t count = lst.size();
        if (count == 0) {
            writeEmptyList();
            return;
        }
        writeListHeader(count);
        for (const auto &e : lst) {
            writeValue(e);
        }
        writeListFooter();
    }

    template<size_t N>
    inline void writeList(const uint8_t (&v)[N]) {
        writeBytes(v, N);
    }

    template<size_t N>
    inline void writeList(const std::array<uint8_t, N> &a) {
        writeBytes(a.data(), a.size());
    }

    template<class Allocator>
    inline void writeList(const std::vector<uint8_t, Allocator> &v) {
        writeBytes(v.data(), v.size());
    }

    template<class T, size_t N>
    void writeList(const T (&v)[N]) {
        skipRef();
        if (N == 0) {
            writeEmptyList();
            return;
        }
        writeListHeader(N);
        for (auto i = 0; i < N; ++i) {
            writeValue(v[i]);
        }
        writeListFooter();
    }

    template<class Allocator>
    void writeList(const std::vector<bool, Allocator> &lst) {
        if (writeRef(lst)) return;
        setRef(lst);
        size_t count = lst.size();
        if (count == 0) {
            writeEmptyList();
            return;
        }
        writeListHeader(count);
        for (const auto &e : lst) {
            writeBool(e);
        }
        writeListFooter();
    }

    template<class T>
    void writeList(const std::forward_list<T> &lst) {
        if (writeRef(lst)) return;
        setRef(lst);
        size_t count = std::distance(std::begin(lst), std::end(lst));
        if (count == 0) {
            writeEmptyList();
            return;
        }
        writeListHeader(count);
        for (const auto &e : lst) {
            writeValue(e);
        }
        writeListFooter();
    }

    template<size_t N>
    void writeList(const std::bitset<N> &b) {
        if (writeRef(b)) return;
        setRef(b);
        if (N == 0) {
            writeEmptyList();
            return;
        }
        writeListHeader(N);
        for (auto i = 0; i < N; ++i) {
            writeBool(b.test(i));
        }
        writeListFooter();
    }

    template<class... Type>
    void writeList(const std::tuple<Type...> &lst) {
        if (writeRef(lst)) return;
        setRef(lst);
        size_t count = std::tuple_size<std::tuple<Type...>>::value;
        if (count == 0) {
            writeEmptyList();
            return;
        }
        writeListHeader(count);
        writeTupleElement(lst);
        writeListFooter();
    }

    template<class T>
    void writeMap(const T &map) {
        if (writeRef(map)) return;
        setRef(map);
        size_t count = map.size();
        if (count == 0) {
            writeEmptyMap();
            return;
        }
        writeMapHeader(count);
        for (const auto &e : map) {
            writeValue(e.first);
            writeValue(e.second);
        }
        writeMapFooter();
    }

    template<class T>
    void writeObject(const T &o) {
        const auto &cache = ClassManager::SharedInstance().getClassCache<T>();
        if (!classRefs) {
            classRefs = std::unique_ptr<std::unordered_map<std::type_index, int>>(
                new std::unordered_map<std::type_index, int>()
            );
        }
        auto type = std::type_index(typeid(T));
        auto iter = classRefs->find(type);
        auto index = 0;
        if (iter == classRefs->end()) {
            stream << cache.data;
            if (refer) {
                refer->addCount(cache.fields.size());
            }
            index = classRefs->size();
            (*classRefs)[type] = index;
        } else {
            index = iter->second;
        }
        if (writeRef(o)) return;
        setRef(o);
        stream << TagObject << index << TagOpenbrace;
        auto fields = cache.fields;
        for (const auto &field : fields) {
            field.encode(&o, *this);
        }
        stream << TagClosebrace;
    }

    template<class T>
    inline bool writeRef(const T &v) {
        return refer ? refer->write(stream, v) : false;
    }

    template<class T>
    inline void setRef(const T &v) {
        if (refer) refer->set(v);
    }

    inline void skipRef() {
        if (refer) refer->set(0);
    }

    inline void reset() {
        if (classRefs) classRefs->clear();
        if (refer) refer->reset();
    }

    std::ostream &stream;

private:
    void writeString(const std::string &str, int length) {
        stream << TagString;
        util::WriteInt(stream, length);
        stream << TagQuote << str << TagQuote;
    }

    inline void writeDate(int year, int month, int day) {
        stream << TagDate;
        util::WriteDate(stream, year, month, day);
    }

    inline void writeTime(int hour, int min, int sec, int nsec) {
        stream << TagTime;
        util::WriteTime(stream, hour, min, sec);
    }

    void writeListHeader(size_t count) {
        stream << TagList;
        util::WriteUint(stream, count);
        stream << TagOpenbrace;
    }

    inline void writeListFooter() {
        stream << TagClosebrace;
    }

    inline void writeEmptyList() {
        stream << TagList << TagOpenbrace << TagClosebrace;
    }

    template<std::size_t Index = 0, class... Tuple>
    inline typename std::enable_if<
        Index == sizeof...(Tuple)
    >::type
    writeTupleElement(const std::tuple<Tuple...> &) {
    }

    template<std::size_t Index = 0, class... Tuple>
    inline typename std::enable_if<
        Index < sizeof...(Tuple)
    >::type
    writeTupleElement(const std::tuple<Tuple...> &tuple) {
        writeValue(std::get<Index>(tuple));
        writeTupleElement<Index + 1, Tuple...>(tuple);
    }

    void writeMapHeader(size_t count) {
        stream << TagMap;
        util::WriteUint(stream, count);
        stream << TagOpenbrace;
    }

    inline void writeMapFooter() {
        stream << TagClosebrace;
    }

    inline void writeEmptyMap() {
        stream << TagMap << TagOpenbrace << TagClosebrace;
    }

    std::unique_ptr<internal::WriterRefer> refer;
    std::unique_ptr<std::unordered_map<std::type_index, int>> classRefs;
};

}
} // hprose::io

#include <hprose/io/Writer-inl.h>
