
#include "core/spinup.hpp"

#include "arch/core/cpulocal-pre.hpp"
#include "core/cpulocal.hpp"

#include <cstdint>

using namespace x86_64;

namespace core::spinup {

static seg::Gdt gdt;

void cpu_spinup() {
    auto cpulocal = core::cpulocal::CpuLocal::get();

    struct [[gnu::packed]] {
        std::uint16_t limit;
        seg::Gdt     *base;
    } gdtr = {
        .limit = sizeof(seg::Gdt) - 1,
        .base  = &gdt,
    };
    __asm__("lgdt %0" ::"m"(gdtr));

    // Reload all the segments from the new GDT.
    __asm__( //
        "mov ax, %0;"
        "mov ds, ax;"
        "mov ss, ax;"
        "mov es, ax;"
        "mov fs, ax;"
        "mov gs, ax;"
        // Not using a direct `jmp` here as that would not be PIC.
        "lea rax, [1f+rip];"
        "push %1;"
        "push rax;"
        "retfq;"
        "1:"
        :
        : "i"(seg::KDATA_SEL), "i"(seg::KCODE_SEL)
        : "rax", "memory"
    );

    // TODO: Set up the TSS.
}

} // namespace core::spinup
