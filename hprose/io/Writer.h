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
 * LastModified: Oct 19, 2016                             *
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
    typedef std::true_type SignedType;
    typedef std::false_type UnignedType;

    Writer(std::ostream &stream, bool simple = false)
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
    inline void writeInteger(T i) {
        static_assert(NonCVType<T>::value == IntegerType::value, "Requires integer type");
        writeInteger(i, std::is_signed<T>());
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
    void writeComplex(const std::complex<T> &c) {
        if (c.imag() == 0) {
            writeFloat(c.real());
            return;
        }
        setRef(0);
        writeListHeader(2);
        writeFloat(c.real());
        writeFloat(c.imag());
        writeListFooter();
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
            writeBytes(reinterpret_cast<const uint8_t *>(str.data()), str.length());
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

    void writeBytes(const uint8_t *data, size_t count) {
        setRef(0);
        if (count == 0) {
            stream << tags::TagBytes << tags::TagQuote << tags::TagQuote;
            return;
        }
        stream << tags::TagBytes;
        util::WriteUint(stream, count);
        stream << tags::TagQuote;
        stream.write(reinterpret_cast<const char *>(data), count);
        stream << tags::TagQuote;
    }

    template<typename T>
    void writeList(const T &lst) {
        static_assert(NonCVType<T>::value == ListType::value, "Requires list type");
        if (writeRef(reinterpret_cast<uintptr_t>(&lst))) {
            return;
        }
        setRef(reinterpret_cast<uintptr_t>(&lst));
        size_t count = lst.size();
        if (count == 0) {
            writeEmptyList();
            return;
        }
        writeListHeader(count);
        for (auto itr = lst.cbegin(); itr != lst.cend(); ++itr) {
            writeValue(*itr);
        }
        writeListFooter();
    }

    template<size_t Size>
    void writeList(const std::array<uint8_t, Size> &a) {
        writeBytes(a.data(), Size);
    }

    template<typename Allocator>
    void writeList(const std::vector<uint8_t, Allocator> &v) {
        writeBytes(v.data(), v.size());
    }

    template<typename Allocator>
    void writeList(const std::vector<bool, Allocator> &lst) {
        if (writeRef(reinterpret_cast<uintptr_t>(&lst))) {
            return;
        }
        setRef(reinterpret_cast<uintptr_t>(&lst));
        size_t count = lst.size();
        if (count == 0) {
            writeEmptyList();
            return;
        }
        writeListHeader(count);
        for (auto itr = lst.cbegin(); itr != lst.cend(); ++itr) {
            writeBool(*itr);
        }
        writeListFooter();
    }

    template<typename T>
    void writeList(const std::forward_list<T> &lst) {
        if (writeRef(reinterpret_cast<uintptr_t>(&lst))) {
            return;
        }
        setRef(reinterpret_cast<uintptr_t>(&lst));
        size_t count = std::distance(std::begin(lst), std::end(lst));
        if (count == 0) {
            writeEmptyList();
            return;
        }
        writeListHeader(count);
        for (auto itr = lst.cbegin(); itr != lst.cend(); ++itr) {
            writeValue(*itr);
        }
        writeListFooter();
    }

    template<typename... Type>
    void writeList(const std::tuple<Type...> &lst) {
        if (writeRef(reinterpret_cast<uintptr_t>(&lst))) {
            return;
        }
        setRef(reinterpret_cast<uintptr_t>(&lst));
        size_t count = std::tuple_size<std::tuple<Type...> >::value;
        if (count == 0) {
            writeEmptyList();
            return;
        }
        writeListHeader(count);
        writeTupleElement(lst);
        writeListFooter();
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
    inline void writeValue(const T &c, ComplexType) {
        writeComplex(c);
    }

    template<typename T>
    inline void writeValue(const T &s, StringType) {
        writeString(s);
    }

    template<typename T>
    inline void writeValue(const T &l, ListType) {
        writeList(l);
    }

    template<typename T>
    void writeInteger(T i, const SignedType &) {
        if (i >= 0 && i <= 9) {
            stream << static_cast<char>('0' + i);
            return;
        }
        if (i >= std::numeric_limits<int32_t>::min() && i <= std::numeric_limits<int32_t>::max()) {
            stream << tags::TagInteger;
        } else {
            stream << tags::TagLong;
        }
        util::WriteInt(stream, i);
        stream << tags::TagSemicolon;
    }

    template<typename T>
    void writeInteger(T u, const UnignedType &) {
        if (u <= 9) {
            stream << static_cast<char>('0' + u);
            return;
        }
        if (u <= std::numeric_limits<int32_t>::max()) {
            stream << tags::TagInteger;
        } else {
            stream << tags::TagLong;
        }
        util::WriteUint(stream, u);
        stream << tags::TagSemicolon;
    }

    void writeString(const std::string &str, int length) {
        stream << tags::TagString;
        util::WriteInt(stream, length);
        stream << tags::TagQuote << str << tags::TagQuote;
    }

    void writeListHeader(size_t count) {
        stream << tags::TagList;
        util::WriteUint(stream, count);
        stream << tags::TagOpenbrace;
    }

    inline void writeListFooter() {
        stream << tags::TagClosebrace;
    }

    inline void writeEmptyList() {
        stream << tags::TagList << tags::TagOpenbrace << tags::TagClosebrace;
    }

    template<std::size_t Index = 0, typename... Tuple>
    inline typename std::enable_if<Index == sizeof...(Tuple), void>::type
    writeTupleElement(const std::tuple<Tuple...> &) {
    }

    template<std::size_t Index = 0, typename... Tuple>
    inline typename std::enable_if<Index < sizeof...(Tuple), void>::type
    writeTupleElement(const std::tuple<Tuple...> &tuple) {
        writeValue(std::get<Index>(tuple));
        writeTupleElement<Index + 1, Tuple...>(tuple);
    }

    std::unique_ptr<internal::WriterRefer> refer;
};

}
} // hprose::io
