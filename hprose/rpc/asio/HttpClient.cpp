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

std::string HttpClient::sendAndReceive(const std::string &request, const ClientContext &context) {
    return std::string();
}

}
}
} // hprose::rpc::asio
