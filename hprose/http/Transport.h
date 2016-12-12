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
 * hprose/http/Transport.h                                *
 *                                                        *
 * hprose http transport for cpp.                         *
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

class Transport {
public:
    virtual Response sendRequest(const Request &req) = 0;

};

}
} // hprose::http
