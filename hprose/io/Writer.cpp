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
 * hprose/io/Writer.cpp                                   *
 *                                                        *
 * hprose writer unit for cpp.                            *
 *                                                        *
 * LastModified: Oct 17, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#include <hprose/io/Writer.h>

namespace hprose {
namespace io {
namespace internal {

bool WriterRefer::write(std::ostream &stream, uintptr_t ptr) {
    auto r = ref.find(ptr);
    if (r != ref.end()) {
        stream << tags::TagRef;
        util::WriteInt(stream, r->second);
        stream << tags::TagSemicolon;
        return true;
    }
    return false;
}

}
}
} // hprose::io::internal
