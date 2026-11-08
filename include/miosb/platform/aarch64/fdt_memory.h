#ifndef MEMORY_REGIONS_H
#define MEMORY_REGIONS_H

#include "miosb/types.h"

typedef struct {
    u64 base,
    size
} mem_region_t;


void fdt_find_memory_regions(void *fdt);

#endif /* MEMORY_REGIONS_H */

