
#pragma once

#include "arch/core/cpulocal-pre.hpp"

namespace core::sched {
struct Thread;
}



namespace core::cpulocal {

struct CpuLocal {
    ArchCpuLocal   arch;
    sched::Thread *thread;

    static inline CpuLocal *get();

    static inline void set(CpuLocal *ptr);
};

} // namespace core::cpulocal

#include "arch/core/cpulocal.hpp"
