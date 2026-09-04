
#include "core/cpulocal.hpp"
#include "core/sched.hpp"
#include "core/spinup.hpp"
#include "init/init.hpp"

#include <cstdint>

using namespace core::sched;
using namespace core::cpulocal;

[[gnu::noreturn]]
static void jump_to_init(void *p) {
    (void)p;
    init::init();
    core::sched::Scheduler::idle_thread_func(nullptr);
}

[[gnu::aligned(8)]]
std::uint8_t     init_stack[32768];
static Thread    bsp_idle     = Thread::uninitialized();
static Scheduler bsp_sched    = Scheduler::uninitialized();
static CpuLocal  bsp_cpulocal = CpuLocal();

extern "C" void _start() {
    CpuLocal::set(&bsp_cpulocal);
    core::spinup::cpu_spinup();
    bsp_idle  = Thread(jump_to_init, nullptr, (void *)(init_stack + sizeof(init_stack)));
    bsp_sched = Scheduler(&bsp_idle);
    bsp_sched.exec();
}
