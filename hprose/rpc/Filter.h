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
 * hprose/rpc/Filter.h                                    *
 *                                                        *
 * hprose rpc filter for cpp.                             *
 *                                                        *
 * LastModified: Dec 14, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

namespace hprose {
namespace rpc {

class Filter {
    virtual std::string inputFilter(const std::string &data, const Context &context) = 0;

    virtual std::string outputFilter(const std::string &data, const Context &context) = 0;
};

}
} // hprose::rpc
