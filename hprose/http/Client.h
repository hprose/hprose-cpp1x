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
 * LastModified: Nov 29, 2017                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <hprose/http/Request.h>
#include <hprose/http/Response.h>
#include <hprose/http/CookieJar.h>

#include <stdexcept>
#include <memory>

namespace hprose {
namespace http {

namespace internal {

bool shouldRedirectGet(int statusCode);

bool shouldRedirectPost(int statusCode);

} // internal

template<class Transport>
class Client {
public:
    Client()
        : cookieJar(nullptr), timeout(0) {
    }

    Transport &getTransport() { return transport; }
    std::shared_ptr<CookieJar> getCookieJar() { return cookieJar; }
    int getTimeout() { return timeout; }

    void setCookieJar(std::shared_ptr<CookieJar> cookieJar) { this->cookieJar = cookieJar; }
    void setTimeout(int timeout) { this->timeout = timeout; }

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

    inline Response get(std::string url) {
        Request req("GET", url);
        return doFollowingRedirects(req, internal::shouldRedirectGet);
    }

    inline Response post(std::string url, std::string bodyType, std::string body) {
        Request req("POST", url, body);
        req.header.set("Content-Type", bodyType);
        return doFollowingRedirects(req, internal::shouldRedirectPost);
    }

    inline Response head(std::string url) {
        Request req("HEAD", url);
        return doFollowingRedirects(req, internal::shouldRedirectGet);
    }

private:
    Response doFollowingRedirects(const Request &req, bool shouldRedirect(int)) {
        auto deadline = this->deadline();
        std::vector<Request> reqs;
        Response resp;
        while (true) {
            if (!reqs.empty()) {
                auto loc = resp.header.get("Location");
                if (loc.empty()) {
                    throw std::runtime_error(std::to_string(resp.statusCode) + " response missing Location header");
                }
            }
            reqs.push_back(req);
            resp = send(req, deadline);
            if (!shouldRedirect(resp.statusCode)) {
                return resp;
            }
        }
    }

    Response send(const Request &req, time_t deadline) {
        return transport.sendRequest(req, deadline);
    }

    time_t deadline() {
        if (timeout > 0) {
            return time(0) + timeout;
        }
        return time_t();
    }

    Transport transport;
    std::shared_ptr<CookieJar> cookieJar;
    int timeout;
};

}
} // hprose::http
