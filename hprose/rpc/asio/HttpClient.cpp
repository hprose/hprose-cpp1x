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
 * hprose/rpc/asio/HttpClient.cpp                         *
 *                                                        *
 * hprose asio http client for cpp.                       *
 *                                                        *
 * LastModified: Dec 9, 2016                              *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#include <hprose/rpc/asio/HttpClient.h>

namespace hprose {
namespace rpc {
namespace asio {

namespace http {

using ::asio::ip::tcp;

Response http::HttpClient::execute(const Request &request) {
    tcp::resolver resolver(ios);
    tcp::socket socket(ios);
    tcp::resolver::query query(request.uri.getHost(), std::to_string(request.uri.getPort()));
    tcp::resolver::iterator iterator = resolver.resolve(query);
    tcp::resolver::iterator end;
    ::asio::error_code error(::asio::error::host_not_found);
    while (error && iterator != end) {
        socket.close();
        socket.connect(*iterator++, error);
    }
    ::asio::streambuf header;
    std::iostream headerStream(&header);
    headerStream << "POST " << ((request.uri.getPath() == "/*") ? std::string("*") : request.uri.getPath()) << " HTTP/1.1\r\n";
    headerStream << "Host: " << request.uri.getHost()
                 << ((request.uri.getPort() == 80) ? std::string() : (':' + std::to_string(request.uri.getPort()))) << "\r\n";
    for (auto iter = request.header.cbegin(); iter != request.header.cend(); iter++) {
        headerStream << iter->first << ": " << iter->second << "\r\n";
    }
//    std::string cookie = CookieManager::SharedInstance()->GetCookie(aliveHost, path, secure);
//    if (!cookie.empty()) {
//        headerStream << "Cookie: " << cookie << "\r\n";
//    }
    headerStream << "Content-Length: " << request.contentLength << "\r\n\r\n";
    if (request.contentLength >= 64 * 1024) {
        ::asio::write(socket, header);
        ::asio::write(socket, ::asio::buffer(request.body));
    } else {
        headerStream << request.body;
        ::asio::write(socket, header);
    }
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

    Response response;
    response.body = responseStream.str();
    return response;
}

} // http

std::string HttpClient::sendAndReceive(const std::string &data, const ClientContext &context) {
    http::Request req(uri);
    req.method = "POST";
    req.body = data;
    req.contentLength = data.size();
    req.header["Content-Type"] = "application/hprose";
    req.header["Connection"] = "close";
    client.timeout = context.settings.timeout;
    auto response = client.execute(req);
    return response.body;
}

}
}
} // hprose::rpc::asio
