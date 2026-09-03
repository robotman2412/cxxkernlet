
#pragma once

#include <cstddef>



namespace init {

// An initgraph task.
struct Task {
    char const *name;
    void (*func)();
    Task *const *deps;
    std::size_t  deps_len;
    bool         finished;
};

// Initialize the kernel and start userspace.
void init();

// Declare an initgraph task with an optional function to run when all dependencies are met.
#define INITGRAPH_TASK(name_, func_, ...)                                                                              \
    [[gnu::section(".initgraph"), gnu::used]] ::init::Task name_ = {                                                   \
        .name     = #name_,                                                                                            \
        .func     = (func_),                                                                                           \
        .deps     = (Task *const[]){__VA_ARGS__},                                                                      \
        .deps_len = sizeof((Task *const[]){__VA_ARGS__}) / sizeof(Task *),                                             \
        .finished = false,                                                                                             \
    };

} // namespace init
