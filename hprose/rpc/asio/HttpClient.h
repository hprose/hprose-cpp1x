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
 * LastModified: Dec 9, 2016                              *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <hprose/Uri.h>
#include <hprose/rpc/Client.h>

#include <asio.hpp>

#include <sstream>


namespace hprose {
namespace rpc {
namespace asio {

namespace http {

typedef std::unordered_map<std::string, std::string> Header;

struct Request {
    explicit Request(const std::string uri)
        : uri(Uri(uri)) {
    }

    std::string method;
    Uri uri;
    std::string proto;
    Header header;
    std::string body;
    size_t contentLength;
};

struct Response {
    Header header;
    std::string body;
};

class HttpClient {
public:
    Response execute(const Request &request);

    int timeout;

private:
    ::asio::io_service ios;
};

}

class HttpClient : public Client {
public:
    HttpClient(const std::string &uri)
        : Client(uri) {
    }

    http::Header header;

protected:
    std::string sendAndReceive(const std::string &data, const ClientContext &context);

private:
    http::HttpClient client;
};

}
}
} // hprose::rpc::asio
