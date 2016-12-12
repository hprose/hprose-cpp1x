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
 * hprose/http/Response.h                                 *
 *                                                        *
 * hprose http response for cpp.                          *
 *                                                        *
 * LastModified: Dec 12, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <string>

namespace hprose {
namespace http {

struct Response {
    std::string status;
    int statusCode;
    Header header;
    std::string body;
};

}
} // hprose::http
