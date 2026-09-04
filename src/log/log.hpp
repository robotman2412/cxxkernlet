
#pragma once

#include <cstddef>
#include <stdarg.h>



namespace log {

void putc(int c);
void puts(char const *str);
void write(char const *msg, std::size_t len);
[[gnu::format(printf, 1, 2)]]
void printf(char const *fmt, ...);
void vprintf(char const *fmt, va_list vl);

} // namespace log
