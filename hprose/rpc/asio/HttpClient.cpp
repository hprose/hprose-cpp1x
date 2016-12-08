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
 * hprose/rpc/asio/HttpClient.cpp                         *
 *                                                        *
 * hprose asio http client for cpp.                       *
 *                                                        *
 * LastModified: Dec 8, 2016                              *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#include <hprose/rpc/asio/HttpClient.h>

namespace hprose {
namespace rpc {
namespace asio {

namespace http {

Response http::HttpClient::execute(const Request &request) {
    Response response;
    return response;
}

} // http

std::string HttpClient::sendAndReceive(const std::string &data, const ClientContext &context) {
    http::Request req;
    req.method = "POST";
    req.url = uri;
    req.body = data;
    req.contentLength = data.size();
    req.header["Content-Type"] = "application/hprose";
    client.timeout = context.settings.timeout;
    client.execute(req);
    return std::string();
}

}
}
} // hprose::rpc::asio
