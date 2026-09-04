
#include <cstdint>

namespace x86_64::seg {

enum Ring : std::uint8_t {
    KERNEL = 0,
    USER   = 3,
};

enum SegType : std::uint8_t {
    CODE      = 0x1e,
    RODATA    = 0x10,
    DATA      = 0x12,
    DATA_DOWN = 0x16,

    LDT       = 0x02,
    TSS_AVAIL = 0x09,
    TSS_BUSY  = 0x0b,
    CALL_GATE = 0x0c,
    IRQ_GATE  = 0x0e,
    TRAP_GATE = 0x0f,
};

struct Tss;



// GDT index: Kernel code.
static constexpr std::uint16_t KCODE_INDEX = 1;
// GDT index: Kernel data/stack.
static constexpr std::uint16_t KDATA_INDEX = 2;
// GDT index: User code (64-bit).
static constexpr std::uint16_t UCODE_INDEX = 3;
// GDT index: User data/stack (64-bit).
static constexpr std::uint16_t UDATA_INDEX = 4;
// GDT index: Task State Segment.
static constexpr std::uint16_t TSS_INDEX   = 5;

static consteval std::uint16_t selector(std::uint16_t index, bool use_ldt, Ring rpl) {
    return index << 3 | (std::uint16_t)use_ldt << 2 | (std::uint16_t)rpl;
}

// GDT index: Kernel code.
static constexpr std::uint16_t KCODE_SEL = selector(KCODE_INDEX, false, Ring::KERNEL);
// GDT index: Kernel data/stack.
static constexpr std::uint16_t KDATA_SEL = selector(KDATA_INDEX, false, Ring::KERNEL);
// GDT index: User code (64-bit).
static constexpr std::uint16_t UCODE_SEL = selector(UCODE_INDEX, false, Ring::USER);
// GDT index: User data/stack (64-bit).
static constexpr std::uint16_t UDATA_SEL = selector(UDATA_INDEX, false, Ring::USER);
// GDT index: Task State Segment.
static constexpr std::uint16_t TSS_SEL   = selector(TSS_INDEX, false, Ring::KERNEL);

// Generic segment descriptor.
struct GenericDesc {
    std::uint16_t limit0;
    std::uint16_t addr0;
    std::uint8_t  addr1;

    std::uint8_t type    : 5;
    std::uint8_t dpl     : 2;
    std::uint8_t present : 1;

    std::uint8_t limit1   : 4;
    std::uint8_t software : 1;
    std::uint8_t code64   : 1;
    std::uint8_t d_32bit  : 1;
    std::uint8_t grain    : 1;

    std::uint8_t addr2;

    void set_addr(std::uint32_t addr) {
        addr0 = addr;
        addr1 = addr >> 16;
        addr2 = addr >> 24;
    }
};

// Extended address descriptors.
struct ExtAddrDesc {
    std::uint32_t addr3;
    std::uint32_t zero;
};

union GdtEntry {
    std::uint64_t null_desc;
    GenericDesc   generic;
    ExtAddrDesc   ext_addr;
};

// The GDT as laid out by this kernel.
struct Gdt {
    GdtEntry entries[7];

    constexpr Gdt() {
        entries[0].null_desc         = 0;
        entries[KCODE_INDEX].generic = GenericDesc{
            .type    = SegType::CODE,
            .dpl     = Ring::KERNEL,
            .present = true,
            .code64  = true,
        };
        entries[KDATA_INDEX].generic = GenericDesc{
            .type    = SegType::DATA,
            .dpl     = Ring::KERNEL,
            .present = true,
        };
        entries[UCODE_INDEX].generic = GenericDesc{
            .type    = SegType::CODE,
            .dpl     = Ring::USER,
            .present = true,
            .code64  = true,
        };
        entries[UDATA_INDEX].generic = GenericDesc{
            .type    = SegType::DATA,
            .dpl     = Ring::USER,
            .present = true,
        };
        entries[TSS_INDEX].generic = GenericDesc{
            .type    = SegType::TSS_AVAIL,
            .dpl     = Ring::KERNEL,
            .present = true,
        };
        entries[TSS_INDEX + 1].ext_addr = ExtAddrDesc{
            .zero = 0,
        };
    }
};



// The task state segment layout used by this kernel.
struct [[gnu::packed, gnu::aligned(4)]] Tss {
    std::uint32_t _resvd0;
    // RSP starting at 0.
    std::uint64_t rsp[4];
    // ISTs; index 0 is reserved.
    std::uint64_t ist[8];
    std::uint32_t _resvd1[3];
    // I/O permission bitmap base.
    std::uint32_t iopb;
    // This kernel does not use the IOPB.
};

} // namespace x86_64::seg
