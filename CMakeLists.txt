
cmake_minimum_required(VERSION 4.4)

project(C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

add_compile_options(
    -fPIC
    --target=x86_64-unknown-elf
    -ffreestanding
    -fno-exceptions
    -fno-rtti
    -nostdinc
    -std=gnu++23
)

add_link_options(
    --target=x86_64-unknown-elf
    -nodefaultlibs -nostartfiles
    -Wl,--build-id=none
    -T${CMAKE_CURRENT_LIST_DIR}/x86_64.ld
)

add_executable(kernel
    src/core/sched.cpp
    src/presched.cpp
)

include_directories(src
    lib/limine
    -isystem lib/freestanding-c-hdrs/include
    -isystem lib/freestanding-cxx-hdrs/x86_64/include
)

link_libraries(compiler-rt)
