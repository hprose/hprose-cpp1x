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
 * hprose/http/asio/Transport.cpp                         *
 *                                                        *
 * hprose http asio transport for cpp.                    *
 *                                                        *
 * LastModified: Dec 12, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#include <hprose/http/asio/Transport.h>

namespace hprose {
namespace http {
namespace asio {

using ::asio::ip::tcp;

Response Transport::sendRequest(const Request &req) {
    tcp::socket socket(ios);
    tcp::resolver resolver(ios);
    uint16_t port = req.uri.getPort();
    tcp::resolver::query query(req.uri.getHost(), port ? std::to_string(port) : req.uri.getScheme());
    ::asio::error_code error;
    ::asio::connect(socket, resolver.resolve(query), error);

    ::asio::streambuf header;
    std::iostream headerStream(&header);
    headerStream << req.method << " "
                 << ((req.uri.getPath() == "/*") ? std::string("*") : req.uri.getPath()) << " "
                 << req.proto << "\r\n";
    headerStream << "Host: " << req.uri.getHost()
                 << ((req.uri.getPort() == 0) ? std::string() : (':' + std::to_string(req.uri.getPort())))
                 << "\r\n";
    for (auto iter = req.header.cbegin(); iter != req.header.cend(); iter++) {
        headerStream << iter->first << ": " << iter->second[0] << "\r\n";
    }
//    std::string cookie = CookieManager::SharedInstance()->GetCookie(aliveHost, path, secure);
//    if (!cookie.empty()) {
//        headerStream << "Cookie: " << cookie << "\r\n";
//    }
    headerStream << "Content-Length: " << req.contentLength << "\r\n\r\n";
    if (req.contentLength >= 64 * 1024) {
        ::asio::write(socket, header);
        ::asio::write(socket, ::asio::buffer(req.body));
    } else {
        headerStream << req.body;
        ::asio::write(socket, header);
    }

    Response resp;
    std::string s;
    size_t bytes = 0, len = 0;
    bool toclose = false, chunked = false;
    while ((bytes = ::asio::read_until(socket, header, "\r\n")) > 2) {
        headerStream >> s;
        if (strcmp(s.c_str(), "Content-Length:") == 0) {
            headerStream >> len;
        } else if (strcmp(s.c_str(), "Connection:") == 0) {
            headerStream >> s;
            if (strcmp(s.c_str(), "close") == 0) {
                toclose = true;
            }
        } else if (strcmp(s.c_str(), "Keep-Alive:") == 0) {
            std::getline(headerStream, s, '=');
            if (strcmp(s.c_str(), " timeout") == 0) {
                clock_t timeout;
                headerStream >> timeout;
//                aliveTime = clock() + timeout * CLOCKS_PER_SEC;
            }
        } else if (strcmp(s.c_str(), "Transfer-Encoding:") == 0) {
            headerStream >> s;
            if (strcmp(s.c_str(), "chunked") == 0) {
                chunked = true;
            }
        } else if ((strcmp(s.c_str(), "Set-Cookie:") == 0) || (strcmp(s.c_str(), "Set-Cookie2:") == 0)) {
            std::getline(headerStream, s);
//            CookieManager::SharedInstance()->SetCookie(aliveHost, s.substr(1, s.size() - 2));
            continue;
        }
        headerStream.ignore((std::numeric_limits<int>::max)(), '\n');
    }
    header.consume(2);

    std::stringstream responseStream;
    if (chunked) {
        while (true) {
            size_t chunk_size = 0;
            ::asio::read_until(socket, header, "\r\n");
            headerStream >> std::hex >> chunk_size >> std::dec;
            header.consume(2);
            if (chunk_size) {
                bytes = 0;
                while (true) {
                    bytes += ::asio::read_until(socket, header, "\r\n");
                    if (bytes > chunk_size) {
                        for (size_t i = 0; i < chunk_size; i++) {
                            responseStream << (char) headerStream.get();
                        };
                        header.consume(2);
                        break;
                    }
                }
            } else {
                header.consume(2);
                break;
            }
        }
    } else {
        bool nosize = !len;
        size_t n = std::min<size_t>(len, header.size());
        for (size_t i = 0; i < n; ++i, --len) {
            responseStream << (char) headerStream.get();
        };
        if (nosize) {
            len = (std::numeric_limits<int>::max)();
        }
        if (len) {
            char buf[1024];
            auto buffer = ::asio::buffer(buf, 1024);
            while (len) {
                auto n = socket.read_some(::asio::buffer(buf), error);
                if (error) break;
                responseStream.write(buf, n);
                len -= n;
            }
        }
    }
    socket.close();

    resp.body = responseStream.str();
    return resp;
}

}
}
} // hprose::http::asio
