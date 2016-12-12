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
 * hprose/http/Client.h                                   *
 *                                                        *
 * hprose http client for cpp.                            *
 *                                                        *
 * LastModified: Dec 12, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <hprose/http/Request.h>
#include <hprose/http/Response.h>

namespace hprose {
namespace http {

namespace internal {

bool shouldRedirectGet(int statusCode);

bool shouldRedirectPost(int statusCode);

} // internal

template<class Transport>
class Client {
public:
    Response execute(const Request &req) {
        std::string method = req.method.empty() ? "GET" : req.method;
        if (method == "GET" || method == "HEAD") {
            return doFollowingRedirects(req, internal::shouldRedirectGet);
        }
        if (method == "POST" || method == "PUT") {
            return doFollowingRedirects(req, internal::shouldRedirectPost);
        }
        return send(req, deadline());
    }

    Response post(std::string url, std::string bodyType, std::string body) {
        Request req("POST", url, body);
        req.header["Content-Type"] = bodyType;
        return doFollowingRedirects(req, internal::shouldRedirectPost);
    }

    Transport transport;

    int timeout;

private:
    Response doFollowingRedirects(const Request &req, bool shouldRedirect(int)) {
        return send(req, deadline());
    }

    Response send(const Request &req, time_t deadline) {
        return transport.sendRequest(req);
    }

    time_t deadline() {
        if (timeout > 0) {
            return time(0) + timeout;
        }
        return time_t();
    }

};

}
} // hprose::http
