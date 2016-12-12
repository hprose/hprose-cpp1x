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
 * LastModified: Dec 12, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#include <hprose/rpc/asio/HttpClient.h>

namespace hprose {
namespace rpc {
namespace asio {

std::string HttpClient::sendAndReceive(const std::string &data, const ClientContext &context) {
    http::Request req("POST", uri, data);
    req.header.insert(header.begin(), header.end());
    req.header.set("Content-Type", "application/hprose");
    client.timeout = context.settings.timeout;
    auto response = client.execute(req);
    return response.body;
}

}
}
} // hprose::rpc::asio
