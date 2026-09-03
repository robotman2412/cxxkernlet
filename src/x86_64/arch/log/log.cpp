
#include "log/log.hpp"

namespace log {

void putc(int c) {
    __asm__("out dx, al" ::"d"(0x3f8), "a"(c));
}

void write(char const *msg, std::size_t len) {
    for (std::size_t i = 0; i < len; i++) {
        putc(msg[i]);
    }
}

} // namespace log
