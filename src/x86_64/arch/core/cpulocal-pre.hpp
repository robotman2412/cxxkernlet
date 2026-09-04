
#pragma once

#include "x86_64/seg.hpp"

using namespace x86_64;

namespace core::cpulocal {

struct CpuLocal;

struct ArchCpuLocal {
    CpuLocal *self_ptr;
    seg::Tss  tss;

    consteval ArchCpuLocal() : self_ptr(nullptr), tss(seg::Tss()) {
    }
};

} // namespace core::cpulocal
