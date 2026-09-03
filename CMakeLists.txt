
cmake_minimum_required(VERSION 4.4)

project(kernlet C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_C_FLAGS "-std=gnu23")
set(CMAKE_CXX_FLAGS "-std=gnu++23 -fno-exceptions -fno-rtti")

set(arch x86_64)

add_compile_options(
    -fPIC
    --target=${arch}-unknown-elf
    -ffreestanding
    -mstack-alignment=8
    -mno-red-zone
    -mgeneral-regs-only
    -masm=intel
    -nostdinc
)

add_link_options(
    -static-pie
    --target=${arch}-unknown-elf
    -nodefaultlibs -nostartfiles
    -Wl,--build-id=none
    -T${CMAKE_CURRENT_LIST_DIR}/${arch}.ld
)

add_executable(kernel
    src/${arch}/arch/core/sched.cpp
    src/${arch}/arch/log/log.cpp
    
    src/core/sched.cpp
    src/init/presched.cpp
    src/init/init.cpp
    src/log/nanoprintf.c
    src/log/log.cpp
)

include_directories(
    src
    src/${arch}
    lib/limine
    lib/nanoprintf
    -isystem lib/system-hdrs
    -isystem lib/freestanding-c-hdrs/include
    -isystem lib/freestanding-cxx-hdrs/${arch}/include
)

link_libraries(compiler-rt)
