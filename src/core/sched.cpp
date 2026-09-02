
#include "sched.hpp"

namespace core::sched {



Thread::Thread(void (*main)(void *), void *cookie, void *stack) {
    // TODO.
}



Scheduler::Scheduler(Thread *idle) {
    // TODO.
}

void Scheduler::exec() {
    while (1);
}



} // namespace core::sched
