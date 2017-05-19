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
 * hprose/io/Reader.h                                     *
 *                                                        *
 * hprose reader header for cpp.                          *
 *                                                        *
 * LastModified: Dec 30, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <hprose/io/ClassManager.h>
#include <hprose/io/RawReader.h>
#include <hprose/io/decoders/BoolDecoder.h>
#include <hprose/io/decoders/IntDecoder.h>
#include <hprose/io/decoders/FloatDecoder.h>
#include <hprose/io/decoders/ComplexDecoder.h>
#include <hprose/io/decoders/StringDecoder.h>
#include <hprose/io/decoders/ListDecoder.h>
#include <hprose/io/decoders/MapDecoder.h>
#include <hprose/io/decoders/ObjectDecoder.h>
#include <hprose/Variant.h>

#include <ctime>
#include <sstream>
#include <memory>
#include <numeric>
#include <locale>
#ifdef HPROSE_HAS_CODECVT
#include <codecvt>
#endif // HPROSE_HAS_CODECVT
#include <typeindex>
#include <type_traits>
#include <vector>
#include <unordered_map>

namespace hprose {
namespace io {

namespace internal {

std::string TagToString(char tag);

class ReaderRefer {
public:
    template<class T>
    inline void set(const T &v) {
        refs.push_back(Variant(v));
    }

    inline const Variant &read(size_t index) {
        return refs[index];
    }

    inline void reset() {
        refs.clear();
    }

private:
    std::vector<Variant> refs;
};

} // hprose::io::internal

template<class T>
struct CastError : std::runtime_error {
    explicit CastError(char tag)
        : std::runtime_error(internal::TagToString(tag) + " can't change to " + typeid(T).name()) {
    }
};

template<class T>
struct Decoder;

class Reader : public RawReader {
public:
    Reader(std::istream &stream, bool simple = false)
        : RawReader(stream), refer(simple ? nullptr : new internal::ReaderRefer()) {
    }

    template<class T>
    inline T unserialize() {
        T v;
        readValue(v);
        return v;
    }

    template<class T>
    inline Reader &unserialize(T &v) {
        readValue(v);
        return *this;
    }

    template<class T>
    inline void readValue(T &v) {
        return decode(v, *this);
    }

    inline bool readBool() {
        return decoders::BoolDecode(*this, static_cast<char>(stream.get()));
    }

    template<class T>
    inline typename std::enable_if<
        std::is_integral<T>::value,
        T
    >::type
    readInteger() {
        return decoders::IntDecode(*this, static_cast<char>(stream.get()));
    }

    template<class T>
    typename std::enable_if<
        std::is_floating_point<T>::value,
        T
    >::type
    readFloat() {
        return decoders::FloatDecode<T>(*this, static_cast<char>(stream.get()));
    }

    template<class T>
    void readComplex(std::complex<T> &v) {
        decoders::ComplexDecode(v, *this, static_cast<char>(stream.get()));
    }

    template<class T>
    inline typename std::enable_if<
        std::is_same<T, std::string>::value,
        T
    >::type
    readString() {
        return decoders::StringDecode(*this, static_cast<char>(stream.get()));
    }

#ifdef HPROSE_HAS_CODECVT
    template<class T>
    inline typename std::enable_if<
        std::is_same<T, std::wstring>::value,
        T
    >::type
    readString() {
#ifndef _MSC_VER
        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        return conv.from_bytes(readString<std::string>());
#else // _MSC_VER
        //codecvt_utf8<wchar_t> works for UTF8->UCS2, this works for UTF8->UTF16
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> conv;
        return conv.from_bytes(readString<std::string>());
#endif // _MSC_VER
    }

    template<class T>
    inline typename std::enable_if<
        std::is_same<T, std::u16string>::value,
        T
    >::type
    readString() {
#ifndef HPROSE_HAS_CODECVT_BUG
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;
        return conv.from_bytes(readString<std::string>());
#else // HPROSE_HAS_CODECVT_BUG
        std::wstring_convert<std::codecvt_utf8_utf16<uint16_t>, uint16_t> conv;
        auto temp = conv.from_bytes(readString<std::string>());
        std::u16string ret;
        ret.resize(temp.size());
        std::copy(temp.cbegin(), temp.cend(), &ret[0]);
        return std::move(ret);
#endif // HPROSE_HAS_CODECVT_BUG
    }

    template<class T>
    inline typename std::enable_if<
        std::is_same<T, std::u32string>::value,
        T
    >::type
    readString() {
#ifndef HPROSE_HAS_CODECVT_BUG
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
        return conv.from_bytes(readString<std::string>());
#else // HPROSE_HAS_CODECVT_BUG
        std::wstring_convert<std::codecvt_utf8<uint32_t>, uint32_t> conv;
        auto temp = conv.from_bytes(readString<std::string>());
        std::u32string ret;
        ret.resize(temp.size());
        std::copy(temp.cbegin(), temp.cend(), &ret[0]);
        return std::move(ret);
#endif // HPROSE_HAS_CODECVT_BUG
    }
#endif // HPROSE_HAS_CODECVT

    template<class T>
    void readList(T &v) {
        decoders::ListDecode(v, *this, static_cast<char>(stream.get()));
    }

    template<class T>
    void readMap(T &v) {
        decoders::MapDecode(v, *this, static_cast<char>(stream.get()));
    }

    template<class T>
    void readObject(T &v) {
        decoders::ObjectDecode(v, *this, static_cast<char>(stream.get()));
    }

    std::string readStringWithoutTag() {
        std::string str = ByteReader::readString();
        setRef(str);
        return str;
    }

    std::tm readDateTimeWithoutTag() {
        std::tm tm;
        tm.tm_year = read4Digit() - 1900;
        tm.tm_mon = read2Digit() - 1;
        tm.tm_mday = read2Digit();
        auto tag = stream.get();
        if (tag == TagTime) {
            tm.tm_hour = read2Digit();
            tm.tm_min = read2Digit();
            tm.tm_sec = read2Digit();
            tag = stream.get();
            if (tag == TagPoint) {
                do {
                    tag = stream.get();
                } while ((tag >= '0') && (tag <= '9'));
            }
        } else {
            tm.tm_hour = 0;
            tm.tm_min = 0;
            tm.tm_sec = 0;
        }
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)

#else
        if (tag == TagUTC) {
            tm.tm_gmtoff = 0;
        } else {
            std::time_t t = time(0);
            std::tm temp;
            localtime_r(&t, &temp);
            tm.tm_isdst = temp.tm_isdst;
            tm.tm_gmtoff = temp.tm_gmtoff;
            tm.tm_zone = temp.tm_zone;
            mktime(&tm);
        }
#endif
        setRef(tm);
        return tm;
    }

    std::tm readTimeWithoutTag() {
        std::tm tm;
        tm.tm_year = 70;
        tm.tm_mon = 0;
        tm.tm_mday = 1;
        tm.tm_hour = read2Digit();
        tm.tm_min = read2Digit();
        tm.tm_sec = read2Digit();
        auto tag = stream.get();
        if (tag == TagPoint) {
            do {
                tag = stream.get();
            } while ((tag >= '0') && (tag <= '9'));
        }
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)

#else
        if (tag == TagUTC) {
            tm.tm_gmtoff = 0;
        } else {
            std::time_t t = time(0);
            std::tm temp;
            localtime_r(&t, &temp);
            tm.tm_isdst = temp.tm_isdst;
            tm.tm_gmtoff = temp.tm_gmtoff;
            tm.tm_zone = temp.tm_zone;
            mktime(&tm);
        }
#endif
        setRef(tm);
        return tm;
    }

    int readCount() {
        return readArithmetic<int>(TagOpenbrace);
    }

    const Variant &readRef() {
        if (!refer) {
            throw std::runtime_error("reference unserialization can't support in simple mode");
        }
        return refer->read(readInt());
    }

    template<class T>
    inline void setRef(const T &v) {
        if (refer) refer->set(v);
    }

    inline void reset() {
        classTypeRef.clear();
        fieldsRef.clear();
        if (refer) refer->reset();
    }

    std::vector<const std::type_info *> classTypeRef;
    std::vector<std::vector<FieldCache>> fieldsRef;

private:
    std::unique_ptr<internal::ReaderRefer> refer;
};

}
} // hprose::io

#include <hprose/io/decoders/FloatDecoder-inl.h>
#include <hprose/io/decoders/ComplexDecoder-inl.h>
#include <hprose/io/decoders/ListDecoder-inl.h>
#include <hprose/io/decoders/MapDecoder-inl.h>
#include <hprose/io/decoders/ObjectDecoder-inl.h>
#include <hprose/io/Reader-inl.h>
