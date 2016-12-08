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
 * hprose/rpc/Client.cpp                                  *
 *                                                        *
 * hprose rpc client for cpp.                             *
 *                                                        *
 * LastModified: Dec 8, 2016                              *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#include <hprose/rpc/Client.h>

namespace hprose {
namespace rpc {

ClientContext Client::getContext(const InvokeSettings *settings) {
    auto context = ClientContext(*this);
    if (settings) {
        context.settings = *settings;
        if (settings->retry <= 0) {
            context.settings.retry = retry;
        }
        if (settings->timeout <= 0) {
            context.settings.timeout = timeout;
        }
    } else {
        context.settings.retry = retry;
        context.settings.timeout = timeout;
    }
    return context;
}

std::string Client::sendRequest(const std::string &request, const ClientContext &context) {
    return sendAndReceive(request, context);
}

}
} // hprose::rpc
