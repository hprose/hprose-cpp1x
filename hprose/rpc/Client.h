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
 * LastModified: Dec 8, 2016                              *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <hprose/rpc/Context.h>

#include <string>
#include <vector>

namespace hprose {
namespace rpc {

class Client;

struct InvokeSettings {
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
    template<class T>
    void invoke(const std::string &name, const std::vector<T> &args, const InvokeSettings *settings = nullptr) {
        auto context = getContext(settings);
    }

protected:
    Client(const std::string uri)
        : uri(uri), retry(10), timeout(30) {
    }

private:
    ClientContext getContext(const InvokeSettings *settings);

    std::string uri;
    std::vector<std::string> uriList;
    int retry;
    int timeout;
};

}
} // hprose::rpc
