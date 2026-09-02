
#pragma once

namespace core::sched {

struct Thread;
struct Scheduler;

struct Thread {
  private:
    void *stack;

    Thread() {
    }

  public:
    // Create new thread.
    Thread(void (*main)(void *), void *cookie, void *stack);
    // Unsafe zero-initialization.
    static Thread uninitialized() {
        return Thread();
    }
};

struct Scheduler {
  private:
    Scheduler() {
    }

  public:
    // Create new scheduler with given idle thread.
    Scheduler(Thread *idle);
    // Unsafe zero-initialization.
    static Scheduler uninitialized() {
        return Scheduler();
    }

    // Start the scheduler on this CPU.
    [[gnu::noreturn]]
    void exec();
};

} // namespace core::sched
