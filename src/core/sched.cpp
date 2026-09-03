
#include "sched.hpp"

namespace core::sched {



Thread::Thread(void (*main)(void *), void *cookie, void *stack) {
    this->stack      = prepare_thread_entry(stack, main, cookie);
    this->next       = nullptr;
    this->no_preempt = 0;
}

[[gnu::noreturn]]
void Thread::exit() {
    __builtin_trap(); // TODO.
}



void Scheduler::exec() {
    void *dummy;
    context_switch(&dummy, &this->idle->stack);
    __builtin_trap();
}

[[gnu::noreturn]]
void Scheduler::idle_thread_func(void *ignored) {
    while (1) {
        yield();
    }
}



void yield() {
    // TODO.
}



} // namespace core::sched
