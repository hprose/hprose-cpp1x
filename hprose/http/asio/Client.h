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
 * hprose/http/asio/Client.h                              *
 *                                                        *
 * hprose http asio client for cpp.                       *
 *                                                        *
 * LastModified: Dec 3, 2017                              *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <hprose/http/Client.h>
#include <hprose/http/asio/Transport.h>

namespace hprose {
namespace http {
namespace asio {

typedef Client<Transport> Client;

}
}
} // hprose::http::asio
