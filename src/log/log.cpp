
#include "log.hpp"

#include "nanoprintf.h"
#include "stdarg.h"

namespace log {

void puts(char const *str) {
    while (*str) {
        putc(*str);
        str++;
    }
}

[[gnu::format(printf, 1, 2)]]
void printf(char const *fmt, ...) {
    va_list l;
    va_start(l, fmt);
    npf_vpprintf([](int c, void *) { putc(c); }, nullptr, fmt, l);
    va_end(l);
}

} // namespace log