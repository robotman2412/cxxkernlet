
#include "misc/panic.hpp"

#include "log/log.hpp"

#include <atomic>
#include <cstdarg>

namespace misc::panic {

static std::atomic<int> panicking = 0;

[[gnu::noreturn]]
static void panic_spin();



void panicf(char const *msg, ...) {
    claim_panic();

    va_list l;
    va_start(l, msg);
    log::vprintf(msg, l);
    va_end(l);

    panic_unchecked();
}

void panic() {
    claim_panic();
    panic_unchecked();
}

void claim_panic() {
    if (panicking.fetch_or(1)) {
        panic_spin();
    }
}

void panic_unchecked() {
    log::puts("**** KERNEL PANIC ****\n");
    panic_spin();
}

void panic_spin() {
    // TODO: Disable interrupts.
    while (1);
}

} // namespace misc::panic
