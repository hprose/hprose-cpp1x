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
 * hprose/rpc/asio/HttpClient.h                           *
 *                                                        *
 * hprose asio http client for cpp.                       *
 *                                                        *
 * LastModified: Nov 29, 2017                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <hprose/Uri.h>
#include <hprose/http/Client.h>
#include <hprose/http/asio/Transport.h>
#include <hprose/rpc/Client.h>

#include <asio.hpp>

namespace hprose {
namespace rpc {
namespace asio {

class HttpClient : public Client {
public:
    HttpClient(const std::string &uri)
        : Client(uri) {
    }

    HttpClient(const std::vector<std::string> &uriList)
        : Client(uriList) {
    }

    inline bool getKeepAlive() {
        return client.getTransport().keepAlive;
    }

    inline void setKeepAlive(bool enable) {
        client.getTransport().keepAlive = enable;
    }

    inline bool getCompression() {
        return client.getTransport().compression;
    }

    inline void setCompression(bool enable) {
        client.getTransport().compression = enable;
    }

    http::Header header;

protected:
    std::string sendAndReceive(const std::string &data, const ClientContext &context);

private:
    http::Client<http::asio::Transport> client;
};

}
}
} // hprose::rpc::asio
