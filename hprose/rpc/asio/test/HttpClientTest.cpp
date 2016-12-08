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
 * HttpClientTest.cpp                                     *
 *                                                        *
 * hprose asio http client test for cpp.                  *
 *                                                        *
 * LastModified: Dec 8, 2016                              *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#include <hprose/rpc/asio/HttpClient.h>

#include <gtest/gtest.h>

using hprose::rpc::InvokeSettings;
using hprose::rpc::asio::HttpClient;

TEST(HttpClient, Basic) {
    HttpClient client("http://hprose.com/examples");
    client.invoke("hello", std::vector<std::string>({"world"}));
}
