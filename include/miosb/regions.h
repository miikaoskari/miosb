#ifndef MIOSB_RESERVED_REGIONS
#define MIOSB_RESERVED_REGIONS

#include "miosb/types.h"
#include <stddef.h>

#define MAX_MEMORY_REGIONS 16

extern char __kernel_start[];
extern char __kernel_end[];

enum mem_kind {
    MEM_USABLE,
    MEM_MMIO,
    MEM_KERNEL,
    MEM_DTB
};

typedef struct {
    u64 base;
    size_t size;
    enum mem_kind kind;
} mem_region_t;

extern mem_region_t g_mem_regions[MAX_MEMORY_REGIONS];
extern int g_mem_region_count;
extern mem_region_t g_mem_reserved_regions[MAX_MEMORY_REGIONS];
extern int g_mem_reserved_region_count;

static inline uptr kernel_start(void) { return (uptr)__kernel_start; }
static inline uptr kernel_end(void) { return (uptr)__kernel_end; }
static inline size_t kernel_size(void) { return (uptr)__kernel_end - (uptr)__kernel_start; }

#endif /* MIOSB_RESERVED_REGIONS */
