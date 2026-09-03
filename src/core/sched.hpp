
#pragma once

#include <cstdint>

namespace core::sched {

struct Thread;
struct Scheduler;

void  context_switch(void **old_sp_out, void *const *new_sp_in);
void *prepare_thread_entry(void *stack_top, void (*main)(void *), void *cookie);

struct Thread {
    friend struct Scheduler;
    friend void yield();

private:
    Thread      *next;
    void        *stack;
    std::int32_t no_preempt;

    consteval Thread() : next(nullptr), stack(nullptr), no_preempt(0) {
    }

public:
    // Create new thread.
    Thread(void (*main)(void *), void *cookie, void *stack);

    // Unsafe zero-initialization.
    static consteval Thread uninitialized() {
        return Thread();
    }
    // Get current thread.
    static inline Thread *current();
    // Terminate current thread immediately.
    [[gnu::noreturn]]
    static void exit();
};

struct Scheduler {
    friend struct Thread;
    friend void yield();

private:
    Thread *queue_head;
    Thread *idle;

    consteval Scheduler() : queue_head(nullptr), idle(nullptr) {
    }

public:
    // Create new scheduler with given idle thread.
    Scheduler(Thread *idle) : queue_head(nullptr), idle(idle) {
    }
    // Unsafe zero-initialization.
    static consteval Scheduler uninitialized() {
        return Scheduler();
    }
    // Thread main for idle threads.
    [[gnu::noreturn]]
    static void idle_thread_func(void *ignored);

    // Start the scheduler on this CPU.
    [[gnu::noreturn]]
    void exec();
};

void yield();

} // namespace core::sched

#include "arch/core/sched.hpp"
