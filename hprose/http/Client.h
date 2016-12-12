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

template<class Transport>
class Client {
public:
    Response execute(const Request &req) {
        return transport.sendRequest(req);
    }

    Transport transport;

    int timeout;
};

}
} // hprose::http
