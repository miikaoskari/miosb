#include <libfdt.h>
#include "miosb/platform/aarch64/barrier.h"
#include <stdint.h>
#include "miosb/driver.h"
#include "miosb/memory.h"
#include "miosb/log.h"
#include "miosb/types.h"
#include "miosb/idmap.h"

volatile u8 *uart = (u8 *) 0x09000000;

void putchar(char c) {
    *uart = c;
}

void print(const char *s)
{
    while(*s != '\0') {
        putchar(*s);
        s++;
    }
}

void kmain(u64 dtb_ptr32, [[maybe_unused]] u64 x1, [[maybe_unused]] u64 x2, [[maybe_unused]] u64 x3)
{
    void *fdt = (void *)(uptr)dtb_ptr32;

    idmap_add_region((u64)(uptr)uart, (u64)(uptr)uart + PAGE_SIZE, MT_DEVICE, false);
    idmap_add_region(dtb_ptr32, dtb_ptr32 + fdt_totalsize(fdt), MT_NORMAL, false);

    init_idmap();

    info("fdt pointer %p", fdt);

    phys_mem_init(9, 9, 9, 9);
    info("physical memory init done");

    struct page *page = alloc_pages(0);

    register_all_drivers();

    probe_all_drivers_from_fdt(fdt);


    for(;;) {
       wfi();
    }
}
