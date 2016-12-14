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

    void setUriList(const std::vector<std::string> &uriList) {
        this->uriList = uriList;
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(this->uriList.begin(), this->uriList.end(), g);
        index = 0;
        failround = 0;
        uri = this->uriList[0];
    }

    inline int getFailround() const {
        return failround;
    }

    template<class R, class T>
    R invoke(const std::string &name, const std::vector<T> &args, const InvokeSettings *settings = nullptr) {
        auto context = getContext(settings);
        auto request = encode(name, args, context);
        auto response = sendRequest(request, context);
        return decode<R>(response, args, context);
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

private:
    ClientContext getContext(const InvokeSettings *settings);

    std::string sendRequest(const std::string &request, const ClientContext &context);

    template<class T>
    std::string encode(const std::string &name, const std::vector<T> &args, const ClientContext &context) {
        std::stringstream stream;
        io::Writer writer(stream, context.settings.simple);
        writer.stream << io::TagCall;
        writer.writeString(name);
        if (args.size() > 0 || context.settings.byref) {
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
    R decode(const std::string &data, const std::vector<T> &args, const ClientContext &context) {
        std::stringstream stream(data);
        io::Reader reader(stream);
        auto tag = reader.stream.get();
        R result;
        if (tag == io::TagResult) {
            reader.unserialize(result);
            tag = reader.stream.get();
            if (tag == io::TagArgument) {

            }
        }
        return result;
    }

    int index;
    int failround;
};

}
} // hprose::rpc
