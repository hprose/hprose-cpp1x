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
 * hprose/io/Tags.h                                       *
 *                                                        *
 * hprose tags constants for cpp.                         *
 *                                                        *
 * LastModified: Dec 12, 2016                             *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#pragma once

namespace hprose {
namespace io {

/* Serialize Tags */
const char TagInteger     = 'i';
const char TagLong        = 'l';
const char TagDouble      = 'd';
const char TagNull        = 'n';
const char TagEmpty       = 'e';
const char TagTrue        = 't';
const char TagFalse       = 'f';
const char TagNaN         = 'N';
const char TagInfinity    = 'I';
const char TagDate        = 'D';
const char TagTime        = 'T';
const char TagUTC         = 'Z';
const char TagBytes       = 'b';
const char TagUTF8Char    = 'u';
const char TagString      = 's';
const char TagGUID        = 'g';
const char TagList        = 'a';
const char TagMap         = 'm';
const char TagClass       = 'c';
const char TagObject      = 'o';
const char TagRef         = 'r';
/* Serialize Marks */
const char TagPos         = '+';
const char TagNeg         = '-';
const char TagSemicolon   = ';';
const char TagOpenbrace   = '{';
const char TagClosebrace  = '}';
const char TagQuote       = '"';
const char TagPoint       = '.';
/* Protocol Tags */
const char TagFunctions   = 'F';
const char TagCall        = 'C';
const char TagResult      = 'R';
const char TagArgument    = 'A';
const char TagError       = 'E';
const char TagEnd         = 'z';

}
} // hprose::io
