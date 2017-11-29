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
 * hprose/http/Request.cpp                                *
 *                                                        *
 * hprose http request for cpp.                           *
 *                                                        *
 * LastModified: Nov 29, 2017                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#include <hprose/http/Request.h>

namespace hprose {
namespace http {

const char *DefaultUserAgent = "Hprose-http-client/1.1";

const std::set<std::string> ReqWriteExcludeHeader = {"Host", "User-Agent", "Content-Length", "Transfer-Encoding", "Trailer"};

inline bool chunked(const std::vector<std::string> &transferEncoding)  { 
    return !transferEncoding.empty() && transferEncoding[0] == "chunked";
}

inline bool isIdentity(const std::vector<std::string> &transferEncoding)  { 
    return transferEncoding.size() == 1 && transferEncoding[0] == "identity";
}

void Request::addCookie(const Cookie &cookie) {
    std::string raw = cookie.getName() + "=" + cookie.getValue();
    std::string cookieValue = header.get("Cookie");
    if (!cookieValue.empty()) {
        header.set("Cookie", cookieValue + "; " + raw);
    } else {
        header.set("Cookie", raw);
    }
}

void Request::write(std::ostream &ostream) const {
    std::string host = uri.getHost();
    if (uri.getPort() > 0) {
        host += ":" + std::to_string(uri.getPort());
    }

    ostream << method << " " << uri.getPath() << " " << proto << "\r\n";

    ostream << "Host: " << host << "\r\n";

    std::string userAgent = DefaultUserAgent;
    if (header.find("User-Agent") != header.end()) {
        userAgent = header.get("User-Agent");
    }
    if (userAgent != "") {
        ostream << "User-Agent: " << host << "\r\n";
    }

    if (close) {
        ostream << "Connection: close\r\n";
    }
    if (shouldSendContentLength()) {
        ostream << "Content-Length: " << contentLength << "\r\n";
    } else if (chunked(transferEncoding)) {
        ostream << "Transfer-Encoding: chunked\r\n";
    }

    header.writeSubset(ostream, ReqWriteExcludeHeader);

    ostream << "\r\n";
}

bool Request::shouldSendContentLength() const {
    if (chunked(transferEncoding)) {
        return false;
    }
    if (contentLength > 0) {
        return true;
    }
    if (contentLength < 0) {
        return false;
    }
    if (method == "POST" || method == "PUT") {
        return true;
    }
    if (contentLength == 0 && isIdentity(transferEncoding)) {
        if (method == "GET" || method == "HEAD") {
            return false;
        }
        return true;
    }
    return false;
}

}
}
