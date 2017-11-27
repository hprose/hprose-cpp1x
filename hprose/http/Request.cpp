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
 * hprose/http/Request.cpp                                *
 *                                                        *
 * hprose http request for cpp.                           *
 *                                                        *
 * LastModified: Nov 27, 2017                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#include <hprose/http/Request.h>

namespace hprose {
namespace http {

const char * DefaultUserAgent = "Hprose-http-client/1.1";

const std::set<std::string> ReqWriteExcludeHeader = {"Host", "User-Agent", "Content-Length", "Transfer-Encoding", "Trailer"};

void Request::write(std::ostream &ostream) const {
    std::string host = uri.getHost();
    if (uri.getPort() > 0) {
        host += ":" + std::to_string(uri.getPort());
    }

    ostream << method << " " << uri.getPath() << " " << proto << "\r\n";

    ostream << "Host: " << host << "\r\n";

    std::string userAgent = DefaultUserAgent;
    if (header.find("User-Agent") != header.end()) {
        userAgent = header.get("User-Agent");
    }
    if (userAgent != "") {
        ostream << "User-Agent: " << host << "\r\n";
    }

    ostream << "Content-Length: " << contentLength << "\r\n";

    header.writeSubset(ostream, ReqWriteExcludeHeader);

    ostream << "\r\n";
}

}
}
