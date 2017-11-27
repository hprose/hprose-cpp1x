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
 * LastModified: Nov 27, 2017                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#include <hprose/http/asio/Transport.h>
#include <hprose/util/Util.h>

namespace hprose {
namespace http {
namespace asio {

using ::asio::ip::tcp;

Response Transport::sendRequest(const Request &req) {
    tcp::socket socket = getConnection(req);
    writeRequest(req, socket);
    return readResponse(socket);
}

tcp::socket Transport::getConnection(const Request &req) {
    tcp::socket socket(ios);
    tcp::resolver resolver(ios);
    uint16_t port = req.uri.getPort();
    tcp::resolver::query query(req.uri.getHost(), port ? std::to_string(port) : req.uri.getScheme());
    ::asio::error_code error;
    ::asio::connect(socket, resolver.resolve(query), error);
    return socket;
}

void Transport::writeRequest(const Request &req, tcp::socket &socket) {
    ::asio::streambuf request;
    std::ostream requestStream(&request);
    req.write(requestStream);
    if (req.contentLength >= 64 * 1024) {
        ::asio::write(socket, request);
        ::asio::write(socket, ::asio::buffer(req.body));
    } else {
        requestStream << req.body;
        ::asio::write(socket, request);
    }
}

Response Transport::readResponse(tcp::socket &socket) {
    Response resp;

    ::asio::streambuf response;
    ::asio::read_until(socket, response, "\r\n");

    std::istream response_stream(&response);
    response_stream >> resp.proto >> resp.statusCode >> resp.status;
    response.consume(2);

    size_t bytes = 0, len = 0;
    bool chunked = false;

    ::asio::read_until(socket, response, "\r\n\r\n");
    std::string header;
    internal::UniEqual uniEqual;
    while (std::getline(response_stream, header) && header != "\r") {
        auto index = header.find_first_of(":");
        if (index != std::string::npos) {
            auto key = header.substr(0, index);
            auto value = header.substr(index + 2, header.size() - index - 3);
            if (uniEqual(key, "Content-Length")) {
                len = std::stoul(value);
                resp.contentLength = len;
            } else if (uniEqual(key, "Transfer-Encoding")) {
                chunked = uniEqual(value, "chunked");
            } else {
                resp.header.set(key, value);
            }
        }
    }

    std::ostringstream responseStream;
    if (chunked) {
        while (true) {
            size_t chunk_size = 0;
            ::asio::read_until(socket, response, "\r\n");
            response_stream >> std::hex >> chunk_size >> std::dec;
            response.consume(2);
            if (chunk_size) {
                bytes = 0;
                while (true) {
                    bytes += ::asio::read_until(socket, response, "\r\n");
                    if (bytes > chunk_size) {
                        for (size_t i = 0; i < chunk_size; i++) {
                            responseStream << (char) response_stream.get();
                        };
                        response.consume(2);
                        break;
                    }
                }
            } else {
                response.consume(2);
                break;
            }
        }
    } else {
        bool nosize = !len;
        size_t n = std::min<size_t>(len, header.size());
        for (size_t i = 0; i < n; ++i, --len) {
            responseStream << (char) response_stream.get();
        };
        if (nosize) {
            len = (std::numeric_limits<int>::max)();
        }
        if (len) {
            char buf[1024];
            ::asio::error_code error;
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
