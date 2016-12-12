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
 * hprose/http/asio/Transport.h                           *
 *                                                        *
 * hprose http asio transport for cpp.                    *
 *                                                        *
 * LastModified: Dec 12, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <hprose/http/Transport.h>

#include <asio.hpp>

namespace hprose {
namespace http {
namespace asio {

class Transport : public http::Transport {
public:
    Response sendRequest(const Request &req);

private:
    ::asio::io_service ios;
};

}
}
} // hprose::http::asio
