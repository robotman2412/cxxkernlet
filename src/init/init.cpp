
#include "init/init.hpp"

#include "log/log.hpp"
#include "misc/panic.hpp"

namespace init {

extern "C" Task __start_initgraph;
extern "C" Task __stop_initgraph;

void init() {
    log::printf("Kernel starting...\n");

    bool progress = false;
    bool work     = true;
    while (work) {
        progress = false;
        work     = false;
        for (auto task = &__start_initgraph; task != &__stop_initgraph; task++) {
            if (!task->finished) {
                bool can_run = true;
                for (std::size_t i = 0; i < task->deps_len; i++) {
                    if (!task->deps[i]->finished) {
                        can_run = false;
                        break;
                    }
                }
                if (can_run) {
                    if (task->func) {
                        log::printf("Task %s...", task->name);
                        task->func();
                        log::printf(" Done.\n");
                    } else {
                        log::printf("Reached stage %s\n", task->name);
                    }
                    task->finished = true;
                    progress       = true;
                } else {
                    work = true;
                }
            }
        }

        if (work && !progress) {
            log::printf("Initgraph deadlock detected:\n");
            for (auto task = &__start_initgraph; task != &__stop_initgraph; task++) {
                if (!task->finished) {
                    log::printf("  %s\n", task->name);
                }
            }
            misc::panic::panic();
        }
    }

    log::printf("Kernel initialized.\n");
}

} // namespace init
