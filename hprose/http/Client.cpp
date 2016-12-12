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
 * hprose/http/Client.h                                   *
 *                                                        *
 * hprose http client for cpp.                            *
 *                                                        *
 * LastModified: Dec 12, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#include <hprose/http/Client.h>
#include <hprose/http/Status.h>

namespace hprose {
namespace http {

namespace internal {

bool shouldRedirectGet(int statusCode) {
    switch (statusCode) {
        case StatusMovedPermanently:
        case StatusFound:
        case StatusSeeOther:
        case StatusTemporaryRedirect:
            return true;
    }
    return false;
}


bool shouldRedirectPost(int statusCode) {
    switch (statusCode) {
        case StatusFound:
        case StatusSeeOther:
            return true;
    }
    return false;
}

} // internal

}
} // hprose::http
