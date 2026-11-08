#include "miosb/platform/aarch64/fdt_memory.h"
#include <libfdt.h>

#define MAX_MEMORY_REGIONS 16

void add_memory_region(void* fdt, int node, int parent, mem_region_t* out, int count);

static mem_region_t mem_regions[MAX_MEMORY_REGIONS];
int mem_region_count;

void add_memory_region(void* fdt, int node, int parent, mem_region_t* out, int count)
{
    int sc = fdt_size_cells(fdt, parent);
    int ac = fdt_address_cells(fdt, parent);

    int len = 0;
    const fdt32_t* reg = fdt_getprop(fdt, node, "reg", &len);

    int stride = sc + ac;
}

/* memory@40000000 {
 *   reg = <0x00 0x40000000 0x00 0x8000000>;
 *   device_type = "memory";
 * };
 * */
void fdt_find_memory_regions(void* fdt)
{
    int node = -1;
    while ((node = fdt_node_offset_by_prop_value(fdt, node, "device_type", "memory", sizeof("memory"))) >= 0) {
        int parent = fdt_parent_offset(fdt, node);

        mem_region_t out;
        add_memory_region(fdt, node, parent, &out, mem_region_count);
    }
}
