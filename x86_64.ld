OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)

ENTRY(_start)

PHDRS
{
    text     PT_LOAD    FLAGS(0x05); /* Execute + Read */
    rodata   PT_LOAD    FLAGS(0x04); /* Read only */
    data     PT_LOAD    FLAGS(0x06); /* Write + Read */
    dynamic  PT_DYNAMIC FLAGS(0x06); /* Dynamic PHDR for relocations */
}

SECTIONS
{
    . = 0xffffffff80000000;

    __start_text = .;
    .text : {
        *(.text .text.*)
    } :text
    . = ALIGN(256);
    .interrupt_vector_table : {
        *(.interrupt_vector_table)
    } :text
    . = ALIGN(4096);
    __stop_text = .;

    __start_rodata = .;
    .rodata : {
        KEEP(*(.requests_start))
        KEEP(*(.requests))
        KEEP(*(.requests_end))
        *(.rodata .rodata.*)
    } :rodata
    .dynamic : { *(.dynamic) } :rodata :dynamic
    .got : { *(.got) } :rodata
    .got.plt : { *(.got.plt) } :rodata
    .dynstr : { *(.dynstr) } :rodata
    .dynsym : { *(.dynsym) } :rodata
    .rela.dyn : { *(.rela.dyn) } :rodata
    . = ALIGN(4096);
    __stop_rodata = .;

    __start_data = .;
    .data : {
        __start_initgraph = .;
        KEEP(*(.initgraph .initgraph.*))
        __stop_initgraph = .;
        *(.data .data.*)
        *(COMMON)
    } :data

    /* Add zero-initialized memory last to avoid padding. */
    .bss : {
        *(.bss .bss.*)
    } :data
    . = ALIGN(4096);
    __stop_data = .;

    /* Discard useless sections. */
    /DISCARD/ : {
        *(.eh_frame*)
        *(.note .note.*)
        *(.interp)
    }
}