
#pragma once

#include "core/cpulocal.hpp"
#include "x86_64/msr.hpp"

#include <cstddef>

namespace core::cpulocal {

CpuLocal *CpuLocal::get() {
    CpuLocal *out;
    __asm__("mov %0, gs:[%1]" : "=r"(out) : "i"(offsetof(CpuLocal, arch.self_ptr)));
    return out;
}

void CpuLocal::set(CpuLocal *ptr) {
    ptr->arch.self_ptr = ptr;
    x86_64::msr::write(x86_64::msr::GSBASE, (std::uint64_t)ptr);
}

} // namespace core::cpulocal
