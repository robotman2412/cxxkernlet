
#pragma once

#include "core/cpulocal.hpp"
#include "core/sched.hpp"

#include <cstddef>

namespace core::sched {

struct Thread;

// Get current thread.
[[gnu::always_inline]]
Thread *Thread::current() {
    Thread *out;
    __asm__("mov %0, gs:[%1]" : "=r"(out) : "i"(offsetof(cpulocal::CpuLocal, thread)));
    return out;
}

} // namespace core::sched
