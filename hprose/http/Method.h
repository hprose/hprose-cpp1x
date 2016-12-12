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

const MethodGet     = "GET";
const MethodHead    = "HEAD";
const MethodPost    = "POST";
const MethodPut     = "PUT";
const MethodPatch   = "PATCH";
const MethodDelete  = "DELETE";
const MethodConnect = "CONNECT";
const MethodOptions = "OPTIONS";
const MethodTrace   = "TRACE";

}
} // hprose::http
