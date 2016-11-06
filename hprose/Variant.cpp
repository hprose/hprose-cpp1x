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
 * hprose/Variant.h                                       *
 *                                                        *
 * variant type for cpp.                                  *
 *                                                        *
 * LastModified: Nov 6, 2016                              *
 * Author: Chen fei <cf@hprose.com>                       *
 *                                                        *
\**********************************************************/

#include <hprose/Variant.h>

namespace hprose {

Variant &Variant::operator=(const Variant &o) {

    return *this;
}

Variant &Variant::operator=(Variant &&o) noexcept {
    if (&o != this) {
        if (type == o.type) {
            switch (type) {
                case String:
                    new (&data.string) std::shared_ptr<std::string>(o.data.string);
                    break;
                case Time:
                    new (&data.time) std::shared_ptr<std::tm>(o.data.time);
                    break;
            }
        } else {
            destroy();
            switch (o.type) {
                case String:
                    new (&data.string) std::shared_ptr<std::string>(std::move(o.data.string));
                    break;
                case Time:
                    new (&data.time) std::shared_ptr<std::tm>(std::move(o.data.time));
                    break;
            }
            type = o.type;
        }
    }
    return *this;
}

void Variant::destroy() noexcept {
    if (type == Null) return;
    switch (type) {
        case String:
            data.string.~shared_ptr();
            break;
        case Time:
            data.time.~shared_ptr();
            break;
    }
}

} // hprose