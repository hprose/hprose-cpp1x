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
 * hprose/http/Request.h                                  *
 *                                                        *
 * hprose http request for cpp.                           *
 *                                                        *
 * LastModified: Dec 12, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <hprose/Uri.h>
#include <hprose/http/Header.h>

#include <string>

namespace hprose {
namespace http {

struct Request {
    explicit Request(const std::string &uri)
        : method("GET"), uri(Uri(uri)), proto("HTTP/1.1") {
    }

    explicit Request(std::string method, const std::string &uri)
        : method(std::move(method)), uri(Uri(uri)), proto("HTTP/1.1") {
    }

    std::string method;
    Uri uri;
    std::string proto;
    Header header;
    std::string body;
    size_t contentLength;
};

}
} // hprose::http
