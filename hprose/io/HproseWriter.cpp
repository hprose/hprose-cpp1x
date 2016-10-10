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
 * HproseWriter.cpp                                       *
 *                                                        *
 * hprose writer unit for cpp.                            *
 *                                                        *
 * LastModified: Oct 10, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#include <hprose/io/HproseWriter.h>

namespace hprose{

void HproseWriter::WriteNull() {
    stream << 'n';
}

}
