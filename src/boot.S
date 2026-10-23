#include "miosb/platform/aarch64/arch.h"

/*
 * arm64 Linux "Image" boot header (Documentation/arch/arm64/booting.rst).
 * has to be the first thing in the linked image.
 * u32 code0;               Executable code
 * u32 code1;               Executable code
 * u64 text_offset;         Image load offset, little endian
 * u64 image_size;          Effective Image size, little endian
 * u64 flags;               kernel flags, little endian
 * u64 res2	= 0;            reserved
 * u64 res3	= 0;            reserved
 * u64 res4	= 0;            reserved
 * u32 magic = 0x644d5241;  Magic number, little endian, "ARM\x64"
 * u32 res5;                reserved (used for PE COFF offset)
 */
.global _start
_start:
    b _kernel                    /* code0 */
    .word 0                      /* code1, unused */
    .quad 0x100000               /* text_offset: __kernel_start - RAM_START */
    .quad __image_size           /* image_size */
    .quad 0xa                    /* flags: LE, placement-anywhere, 4K pages */
    .quad 0                      /* reserved */
    .quad 0                      /* reserved */
    .quad 0                      /* reserved */
    .byte 0x41, 0x52, 0x4d, 0x64 /* magic: "ARM\x64" */
    .word 0                      /* reserved */

_kernel:
    /* save dtb pointer into x19 */
    mov x19, x0
    /* enable floating point */
    bl enable_fp
    /* initialize vector table */
    bl _irq_vector_init
    /* zero out bss section */
    ldr x0, =__bss_start
    ldr x1, =__bss_end
1:  cmp x0, x1
    b.hs 2f
    str xzr, [x0], #8
    b 1b
2:
    /* initialize stack pointer */
    ldr x30, =stack_top
    mov sp, x30
    mov x0, x19
    bl kmain
    b .

enable_fp:
    mrs x0, cpacr_el1
    orr x0, x0, #CPACR_EL1_FPEN(CPACR_EL1_FP_TRAP_NONE)
    msr cpacr_el1, x0
    isb
    ret
