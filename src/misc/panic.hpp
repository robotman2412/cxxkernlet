
#pragma once



namespace misc::panic {

// Initiate a kernel panic; this should be the last resort!
[[gnu::noreturn]]
void panicf(char const *msg, ...);

// Initiate a kernel panic; this should be the last resort!
[[gnu::noreturn]]
void panic();

// If a kernel panic is happening, spin.
// If not, initiate it (to be finished with `panic_unchecked`).
void claim_panic();

// Initiate a kernel panic without checking whether one is already in progress.
// Called after `claim_panic`.
[[gnu::noreturn]]
void panic_unchecked();

// Unreachable code marker; panics on debug build, optimize out on release builds.
[[gnu::noreturn]]
static inline void unreachable() {
#ifdef NDEBUG
    __builtin_unreachable();
#else
    panicf("Unreachable code");
#endif
}

} // namespace misc::panic
