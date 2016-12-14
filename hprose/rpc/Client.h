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
 * hprose/rpc/Client.h                                    *
 *                                                        *
 * hprose rpc client header for cpp.                      *
 *                                                        *
 * LastModified: Dec 14, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <hprose/io/Writer.h>
#include <hprose/io/Reader.h>
#include <hprose/rpc/Context.h>
#include <hprose/rpc/Filter.h>

#include <algorithm>
#include <string>
#include <vector>
#include <random>

namespace hprose {
namespace rpc {

class Client;

enum ResultMode {
    Normal,
    Serialized,
    Raw,
    RawWithEndTag
};

struct InvokeSettings {
    ResultMode mode;
    bool async;
    bool byref;
    bool simple;
    bool idempotent;
    bool failswitch;
    bool oneway;
    int retry;
    int timeout;
};

class ClientContext : Context {
public:
    ClientContext(const Client &client)
        : retried(0), client(client), settings(InvokeSettings()) {
    }

    const Client &getClient() {
        return client;
    }

    int retried;
    InvokeSettings settings;

private:
    const Client &client;
};

class Client {
public:
    inline const std::string &getUri() const {
        return uri;
    }

    inline void setUri(const std::string &uri) {
        setUriList({uri});
    }

    inline const std::vector<std::string> &getUriList() const {
        return uriList;
    }

    void setUriList(std::vector<std::string> uriList) {
        if (uriList.size() > 1) {
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(uriList.begin(), uriList.end(), g);
        }
        index = 0;
        failround = 0;
        uri = uriList[0];
        this->uriList = std::move(uriList);
    }

    inline int getFailround() const {
        return failround;
    }

    template<class R, class T>
    typename std::enable_if<
        !std::is_void<R>::value,
        R
    >::type
    invoke(const std::string &name, const std::vector<T> &args, const InvokeSettings *settings = nullptr) {
        auto context = getContext(settings);
        if (context.settings.oneway) {
            throw std::runtime_error("oneway must return void type");
        }
        auto request = encode(name, args, context);
        auto response = sendRequest(request, context);
        return decode<R>(response, args, context);
    }

    template<class R, class T>
    typename std::enable_if<
        std::is_void<R>::value,
        void
    >::type
    invoke(const std::string &name, const std::vector<T> &args, const InvokeSettings *settings = nullptr) {
        auto context = getContext(settings);
        auto request = encode(name, args, context);
        sendRequest(request, context);
    }

    int retry;
    int timeout;

protected:
    Client(const std::string &uri)
        : retry(10), timeout(30) {
        setUri(uri);
    }

    Client(const std::vector<std::string> &uriList)
        : retry(10), timeout(30) {
        setUriList(uriList);
    }

    virtual std::string sendAndReceive(const std::string &request, const ClientContext &context) = 0;

    std::string uri;
    std::vector<std::string> uriList;
    std::vector<Filter> filters;

private:
    ClientContext getContext(const InvokeSettings *settings);

    std::string sendRequest(const std::string &request, const ClientContext &context);

    template<class T>
    std::string encode(const std::string &name, const std::vector<T> &args, const ClientContext &context) {
        std::stringstream stream;
        io::Writer writer(stream, context.settings.simple);
        writer.stream << io::TagCall;
        writer.writeString(name);
        if (!args.empty() || context.settings.byref) {
            writer.reset();
            writer.writeList(args);
            if (context.settings.byref) {
                writer.writeBool(true);
            }
        }
        writer.stream << io::TagEnd;
        return stream.str();
    }

    template<class R, class T>
    typename std::enable_if<
        !std::is_same<R, std::string>::value,
        R
    >::type
    decode(const std::string &data, const std::vector<T> &args, const ClientContext &context) {
        checkData(data);
        if (context.settings.mode != Normal) {
            throw std::runtime_error("only normal mode can return non string type");
        }

        R result;
        std::stringstream stream(data);
        io::Reader reader(stream);
        auto tag = reader.stream.get();
        if (tag == io::TagResult) {
            reader.unserialize(result);
            tag = reader.stream.get();
            if (tag == io::TagArgument) {

            }
        } else if (tag == io::TagError) {
            throw std::runtime_error(reader.readString<std::string>());
        }
        if (tag != io::TagEnd) {
            throw std::runtime_error("wrong response: \r\n" + data);
        }
        return result;
    }

    template<class R, class T>
    typename std::enable_if<
        std::is_same<R, std::string>::value,
        std::string
    >::type
    decode(const std::string &data, const std::vector<T> &args, const ClientContext &context) {
        checkData(data);
        if (context.settings.mode == RawWithEndTag) {
            return data;
        } else if (context.settings.mode == Raw) {
            return data.substr(0, data.size() - 1);
        }

        std::string result;
        std::stringstream stream(data);
        io::Reader reader(stream);
        auto tag = reader.stream.get();
        if (tag == io::TagResult) {
            if (context.settings.mode == Normal) {
                reader.unserialize(result);
            } else if (context.settings.mode == Serialized) {

            }
            tag = reader.stream.get();
            if (tag == io::TagArgument) {

            }
        } else if (tag == io::TagError) {
            throw std::runtime_error(reader.readString<std::string>());
        }
        if (tag != io::TagEnd) {
            throw std::runtime_error("wrong response: \r\n" + data);
        }
        return result;
    }

    void checkData(const std::string &data) {
        if (data.empty()) {
            throw std::runtime_error("unexpected eof");
        }
        if (data[data.size() - 1] != io::TagEnd) {
            throw std::runtime_error("wrong response: \r\n" + data);
        }
    }

    int index;
    int failround;
};

}
} // hprose::rpc
