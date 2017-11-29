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
 * hprose/http/CookieJar.h                                *
 *                                                        *
 * hprose http cookie jar for cpp.                        *
 *                                                        *
 * LastModified: Nov 29, 2017                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <hprose/Uri.h>
#include <hprose/http/Cookie.h>

#include <vector>

namespace hprose {
namespace http {

class CookieJar {
public:
    virtual std::vector<Cookie> getCookies(const Uri &uri) = 0;

    virtual void setCookies(const Uri &uri, const std::vector<Cookie> &cookies) = 0;
};

}
} // hprose::http
