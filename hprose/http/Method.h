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
 * hprose/http/Method.h                                   *
 *                                                        *
 * hprose http common methods for cpp.                    *
 *                                                        *
 * LastModified: Dec 12, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

namespace hprose {
namespace http {

const char *MethodGet     = "GET";
const char *MethodHead    = "HEAD";
const char *MethodPost    = "POST";
const char *MethodPut     = "PUT";
const char *MethodPatch   = "PATCH";
const char *MethodDelete  = "DELETE";
const char *MethodConnect = "CONNECT";
const char *MethodOptions = "OPTIONS";
const char *MethodTrace   = "TRACE";

}
} // hprose::http
