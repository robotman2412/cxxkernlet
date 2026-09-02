
#include "core/sched.hpp"

#include <cstdint>

using namespace core::sched;

void foo(void *p) {
    while (1);
}

[[gnu::aligned(16)]]
std::uint8_t     init_stack[32768];
static Thread    init      = Thread::uninitialized();
static Scheduler bsp_sched = Scheduler::uninitialized();

extern "C" void _start() {
    init      = Thread(foo, nullptr, (void *)init_stack);
    bsp_sched = Scheduler(&init);
    bsp_sched.exec();
}
