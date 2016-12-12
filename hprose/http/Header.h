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
 * hprose/http/Header.h                                   *
 *                                                        *
 * hprose http header for cpp.                            *
 *                                                        *
 * LastModified: Dec 12, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <string>
#include <unordered_map>

namespace hprose {
namespace http {

typedef std::unordered_map<std::string, std::string> Header;

}
} // hprose::http
