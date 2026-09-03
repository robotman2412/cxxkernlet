
#pragma once

#include <cstdint>



namespace x86_64::msr {

constexpr std::uint32_t EFER    = 0xC0000080;
constexpr std::uint32_t FSBASE  = 0xC0000100;
constexpr std::uint32_t GSBASE  = 0xC0000101;
constexpr std::uint32_t KGSBASE = 0xC0000102;

static inline std::uint64_t read(std::uint32_t index) {
    std::uint32_t low, high;
    __asm__("rdmsr" : "=a"(low), "=d"(high) : "c"(index));
    return ((std::uint64_t)low << 32) | (std::uint64_t)high;
}

static inline void write(std::uint32_t index, std::uint64_t value) {
    std::uint32_t low  = value;
    std::uint32_t high = value >> 32;
    __asm__("wrmsr" ::"a"(low), "d"(high), "c"(index));
}

} // namespace x86_64::msr
