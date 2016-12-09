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
 * LastModified: Dec 9, 2016                              *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <hprose/io/Writer.h>
#include <hprose/io/Reader.h>
#include <hprose/rpc/Context.h>

#include <string>
#include <vector>

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
    template<class R, class T>
    R invoke(const std::string &name, const std::vector<T> &args, const InvokeSettings *settings = nullptr) {
        auto context = getContext(settings);
        auto request = encode(name, args, context);
        auto response = sendRequest(request, context);
        return decode<R>(response, args, context);
    }

protected:
    Client(const std::string uri)
        : uri(uri), retry(10), timeout(30) {
    }

    virtual std::string sendAndReceive(const std::string &request, const ClientContext &context) = 0;

    std::string uri;

private:
    ClientContext getContext(const InvokeSettings *settings);

    std::string sendRequest(const std::string &request, const ClientContext &context);

    template<class T>
    std::string encode(const std::string &name, const std::vector<T> &args, const ClientContext &context) {
        std::stringstream stream;
        io::Writer writer(stream, context.settings.simple);
        writer.stream << io::tags::TagCall;
        writer.writeString(name);
        if (args.size() > 0 || context.settings.byref) {
            writer.reset();
            writer.writeList(args);
            if (context.settings.byref) {
                writer.writeBool(true);
            }
        }
        writer.stream << io::tags::TagEnd;
        return stream.str();
    }

    template<class R, class T>
    R decode(const std::string &data, const std::vector<T> &args, const ClientContext &context) {
        std::stringstream stream(data);
        io::Reader reader(stream);
        auto tag = reader.stream.get();
        R result;
        if (tag == io::tags::TagResult) {
            reader.unserialize(result);
            tag = reader.stream.get();
            if (tag == io::tags::TagArgument) {

            }
        }
        return result;
    }

    std::vector<std::string> uriList;
    int retry;
    int timeout;
};

}
} // hprose::rpc
