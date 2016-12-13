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
 * LastModified: Dec 13, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <hprose/http/Transport.h>

#include <asio.hpp>

namespace hprose {
namespace http {
namespace asio {

using ::asio::ip::tcp;

class Transport : public http::Transport {
public:
    Response sendRequest(const Request &req);

private:
    tcp::socket getConnection(const Request &req);

    void writeRequest(const Request &req, tcp::socket &socket);

    Response readResponse(tcp::socket &socket);

    ::asio::io_service ios;
};

}
}
} // hprose::http::asio
