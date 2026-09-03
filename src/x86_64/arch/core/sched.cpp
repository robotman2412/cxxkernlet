
#include "core/sched.hpp"

namespace core::sched {

[[gnu::naked, gnu::noinline]]
void context_switch(void **old_sp_out, void *const *new_sp_in) {
    __asm__("push rbp;"
            "push rbx;"
            "push r12;"
            "push r13;"
            "push r14;"
            "push r15;"
            "mov qword ptr [rdi], rsp;"
            "mov rsp, qword ptr [rsi];"
            "pop r15;"
            "pop r14;"
            "pop r13;"
            "pop r12;"
            "pop rbx;"
            "pop rbp;"
            "ret;");
}

[[gnu::naked]]
static void trampoline() {
    __asm__("mov rdi, rbx;"
            "call r12;"
            "call %0;"
            "ud2;" ::"m"(Thread::exit));
}

void *prepare_thread_entry(void *stack_top0, void (*main)(void *), void *cookie) {
    std::size_t *stack_top = (std::size_t *)stack_top0;

    *--stack_top = (std::size_t)&trampoline; // return address
    *--stack_top = 0;                        // rbp
    *--stack_top = (std::size_t)cookie;      // rbx
    *--stack_top = (std::size_t)main;        // r12
    *--stack_top = 0;                        // r13
    *--stack_top = 0;                        // r14
    *--stack_top = 0;                        // r15

    return (void *)stack_top;
}

} // namespace core::sched
