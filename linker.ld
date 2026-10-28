ENTRY(_start)
SECTIONS {
    . = 0x40100000; /* RAM starts at 0x40000000 but if we ask to load the kernel there, QEMU will not load a DTB */
    __kernel_start = .;
    .startup . : { *boot*(.text) }
    .text : { *(.text) }
    .rodata : { *(.rodata*) }
    .data : { *(.data) }
    .bss : ALIGN(8) {
        __bss_start = .;
        *(.bss COMMON)
        . = ALIGN(8);
        __bss_end = .;
    }

    .drivers : {
        __drivers_start = .;
        KEEP(*(.drivers))
        __drivers_end = .;
    }

    . = ALIGN(16);
    . += 0x4000; /* 16kB of stack memory */
    stack_top = .;

    __kernel_end = .;

    __image_size = __kernel_end - __kernel_start;
}
