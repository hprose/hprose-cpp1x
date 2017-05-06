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
 * LastModified: Dec 9, 2016                              *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#include <hprose/rpc/asio/HttpClient.h>

#include <gtest/gtest.h>

using hprose::rpc::InvokeSettings;
using hprose::rpc::asio::HttpClient;

TEST(HttpClient, Basic) {
    HttpClient client("http://hprose.com/example/");
    auto result = client.invoke<std::string>("hello", std::vector<std::string>({"world"}));
    EXPECT_EQ(result, "Hello world");
}

TEST(HttpClient, Async) {
    HttpClient client("http://hprose.com/example/");
    auto result = client.invoke<std::future<std::string> >("hello", std::vector<std::string>({"world"}));
    EXPECT_EQ(result.get(), "Hello world");
}
