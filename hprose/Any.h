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
 * hprose/Any.h                                           *
 *                                                        *
 * any type for cpp, ported from boost::any.              *
 *                                                        *
 * LastModified: Dec 4, 2016                              *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <stdexcept>
#include <typeinfo>
#include <type_traits>

namespace hprose {

class Any {
public:
    Any() noexcept
        : content(0) {
    }

    template<typename T>
    Any(const T &value)
        : content(new holder<typename std::remove_cv<typename std::decay<const T>::type>::type>(value)) {
    }

    Any(const Any &other)
        : content(other.content ? other.content->clone() : 0) {
    }

    Any(Any &&other) noexcept
        : content(other.content) {
        other.content = 0;
    }

//    template<typename T>
//    Any(T &&value
//        , typename std::disable_if<std::is_same<any &, T> >::type * = 0
//        , typename std::disable_if<std::is_const<T> >::type * = 0)
//        : content(new holder<typename std::decay<T>::type>(static_cast<T &&>(value))) {
//    }

    ~Any() noexcept {
        delete content;
    }

public:
    Any &swap(Any &rhs) {
        std::swap(content, rhs.content);
        return *this;
    }

    template<typename T>
    Any &operator=(const T &rhs) {
        Any(rhs).swap(*this);
        return *this;
    }

    Any &operator=(Any &&rhs) noexcept {
        rhs.swap(*this);
        Any().swap(rhs);
        return *this;
    }

    template<class ValueType>
    Any &operator=(ValueType &&rhs) {
        Any(static_cast<ValueType &&>(rhs)).swap(*this);
        return *this;
    }

public:
    bool empty() const noexcept {
        return !content;
    }

    void clear() noexcept {
        Any().swap(*this);
    }

    const std::type_info &type() const noexcept {
        return content ? content->type() : typeid(void);
    }

private:

    class placeholder {
    public:

        virtual ~placeholder() {
        }

    public:

        virtual const std::type_info &type() const = 0;

        virtual placeholder *clone() const = 0;
    };

    template<typename T>
    class holder : public placeholder {
    public:

        holder(const T &value)
            : held(value) {
        }

        holder(T &&value)
            : held(static_cast<T &&>(value)) {
        }

    public:

        virtual const std::type_info &type() const {
            return typeid(T);
        }

        virtual placeholder *clone() const {
            return new holder(held);
        }

    public:

        T held;

    private:

        holder &operator=(const holder &);

    };

public:

    template<typename T>
    static T *cast(Any *operand) {
        return operand &&
               operand->type() == typeid(T)
               ? &static_cast<typename Any::holder<T> *>(operand->content)->held
               : 0;
    }

    template<typename T>
    inline static const T *cast(const Any *operand) {
        return cast<T>(const_cast<Any *>(operand));
    }

    template<typename T>
    static T cast(Any &operand) {
        typedef typename std::remove_reference<T>::type nonref;
        nonref *result = cast<nonref>(&operand);
        if (!result) {
            throw std::runtime_error("failed conversion using hprose::Any::cast");
        }
        return *result;
    }

    template<typename T>
    inline static T cast(const Any &operand) {
        typedef typename std::remove_reference<T>::type nonref;
        return cast<const nonref &>(const_cast<Any &>(operand));
    }

    template<typename T>
    inline static T *unsafe_cast(Any *operand) {
        return &static_cast<typename Any::holder<T> * > (operand->content)->held;
    }

    template<typename T>
    inline static const T *unsafe_cast(const Any *operand) {
        return unsafe_cast<T>(const_cast<Any *>(operand));
    }

private:

    placeholder *content;

};


} //hprose
