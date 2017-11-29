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
 * LastModified: Nov 29, 2017                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <hprose/Uri.h>
#include <hprose/http/Header.h>
#include <hprose/http/Cookie.h>

#include <string>
#include <ostream>

namespace hprose {
namespace http {

struct Request {
    explicit Request(const std::string &uri)
        : method("GET"), uri(Uri(uri)), proto("HTTP/1.1"), contentLength(0), close(false) {
    }

    explicit Request(std::string method, const std::string &uri)
        : method(std::move(method)), uri(Uri(uri)), proto("HTTP/1.1"), contentLength(0), close(false) {
    }

    explicit Request(std::string method, const std::string &uri, std::string body)
        : method(std::move(method)), uri(Uri(uri)), proto("HTTP/1.1"), body(std::move(body)),
          contentLength(this->body.size()), close(false) {
    }

    void addCookie(const Cookie &cookie);

    void write(std::ostream &ostream) const;

    std::string method;
    Uri uri;
    std::string proto;
    Header header;
    std::string body;
    int64_t contentLength;
    std::vector<std::string> transferEncoding;
    bool close;

private:
    bool shouldSendContentLength() const;
};

}
} // hprose::http
