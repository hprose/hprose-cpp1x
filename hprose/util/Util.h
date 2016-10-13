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
 * hprose/util/Util.h                                     *
 *                                                        *
 * some util for cpp.                                     *
 *                                                        *
 * LastModified: Oct 13, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

#include <string>

namespace hprose { namespace util {

// UTF16Length return the UTF16 length of str.
// str must be an UTF8 encode string, otherwise return -1.
int UTF16Length(const std::string& str);

} } // hprose::util
