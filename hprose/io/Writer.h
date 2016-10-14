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
 * LastModified: Oct 14, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <hprose/io/Tags.h>
#include <hprose/io/Types.h>
#include <hprose/util/Util.h>

#include <ostream>
#include <numeric>
#include <limits>
#include <locale>
#include <codecvt>
#include <unordered_map>

namespace hprose {
namespace io {

namespace internal {

class WriterRefer {
public:
    inline void set(uintptr_t ptr) {
        ref[ptr] = lastref++;
    }

    bool write(std::ostream &stream, uintptr_t ptr);

    inline void reset() {
        ref.clear();
        lastref = 0;
    }

private:
    std::unordered_map<uintptr_t, int> ref;
    int lastref;
};

} // hprose::io::internal

class Writer {
public:
    Writer(std::ostream &stream, bool simple = true)
        : stream(stream), refer(simple ? nullptr : new internal::WriterRefer()) {
    }

    template<typename T>
    inline Writer &serialize(const T &v) {
        writeValue(v);
        return *this;
    }

    template<typename T>
    inline void writeValue(const T &v) {
        static_assert(NonCVType<T>::value != UnknownType::value, "Attempt to write a value that can not be serializable");
        writeValue(v, NonCVType<T>());
    }

    inline void writeNull() {
        stream << tags::TagNull;
    }

    inline void writeBool(bool b) {
        stream << (b ? tags::TagTrue : tags::TagFalse);
    }

    template<typename T>
    void writeInteger(T i) {
        static_assert(NonCVType<T>::value == IntegerType::value, "Requires integer type");
        if (i >= 0 && i <= 9) {
            stream << static_cast<char>('0' + i);
            return;
        }
        if (i >= std::numeric_limits<int32_t>::min() && i <= std::numeric_limits<int32_t>::max()) {
            stream << tags::TagInteger;
        } else {
            stream << tags::TagLong;
        }
        stream << i << tags::TagSemicolon;
    }

    template<typename T>
    void writeFloat(T f) {
        static_assert(NonCVType<T>::value == FloatType::value, "Requires floating point type");
        if (f != f) {
            stream << tags::TagNaN;
        } else if (f == std::numeric_limits<T>::infinity()) {
            stream << tags::TagInfinity << tags::TagPos;
        } else if (f == -std::numeric_limits<T>::infinity()) {
            stream << tags::TagInfinity << tags::TagNeg;
        } else {
            stream.precision(std::numeric_limits<T>::digits10);
            stream << tags::TagDouble << f << tags::TagSemicolon;
        }
    }

    template<typename T>
    void writeString(const T &s) {
        static_assert(NonCVType<T>::value == StringType::value, "Requires string type");
    }

    void writeString(const std::string &str) {
        int length = util::UTF16Length(str);
        if (length == 0) {
            stream << tags::TagEmpty;
        } else if (length == 1) {
            stream << tags::TagUTF8Char << str;
        } else if (length < 0) {

        } else {
            if (writeRef(reinterpret_cast<uintptr_t>(&str))) {
                return;
            }
            setRef(reinterpret_cast<uintptr_t>(&str));
            writeString(str, length);
        }
    }

    void writeString(const std::wstring &str) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        writeString(conv.to_bytes(str));
    }

    void writeString(const std::u16string &str) {
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;
        writeString(conv.to_bytes(str));
    }

    void writeString(const std::u32string &str) {
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
        writeString(conv.to_bytes(str));
    }

    inline bool writeRef(uintptr_t ptr) {
        return refer ? refer->write(stream, ptr) : false;
    }

    inline void setRef(uintptr_t ptr) {
        if (refer) refer->set(ptr);
    }

    inline void reset() {
        if (refer) refer->reset();
    }

    std::ostream &stream;

private:
    template<typename T>
    inline void writeValue(const T &, NullPtrType) {
        writeNull();
    }

    template<typename T>
    inline void writeValue(const T &b, BoolType) {
        writeBool(b);
    }

    template<typename T>
    inline void writeValue(const T &i, IntegerType) {
        writeInteger(i);
    }

    template<typename T>
    inline void writeValue(const T &f, FloatType) {
        writeFloat(f);
    }

    template<typename T>
    inline void writeValue(const T &s, StringType) {
        writeString(s);
    }

    void writeString(const std::string &str, int length) {
        stream << tags::TagString << length << tags::TagQuote << str << tags::TagQuote;
    }

    std::unique_ptr<internal::WriterRefer> refer;
};

}
} // hprose::io
