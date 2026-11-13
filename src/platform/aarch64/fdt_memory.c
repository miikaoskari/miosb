#include "miosb/platform/aarch64/fdt_memory.h"
#include <libfdt.h>

#define MAX_MEMORY_REGIONS 16

void add_memory_region(void* fdt, int node, int parent, mem_region_t *out, int *count);

static mem_region_t mem_regions[MAX_MEMORY_REGIONS];
int mem_region_count;
static mem_region_t mem_reserved_regions[MAX_MEMORY_REGIONS];
int reserved_region_count;

static u64 read_cells(const fdt32_t *reg, int cells)
{
    u64 v = 0;
    for (int i = 0; i < cells; i++)
        v = (v << 32) | fdt32_to_cpu(reg[i]);
    return v;
}

void add_memory_region(void* fdt, int node, int parent, mem_region_t *out, int *count)
{
    int size_cells = fdt_size_cells(fdt, parent);
    int address_cells = fdt_address_cells(fdt, parent);
    if (size_cells < 0 || address_cells < 0)
        return;

    int len = 0;
    const fdt32_t* reg = fdt_getprop(fdt, node, "reg", &len);
    if (!reg || len <= 0)
        return;

    int stride = size_cells + address_cells;
    int total_cells = len / (int)sizeof(fdt32_t);

    for (int i = 0; i + stride <= total_cells && *count < MAX_MEMORY_REGIONS; i += stride)
    {
        u64 size = read_cells(reg + i + address_cells, size_cells);
        if (size == 0)
            continue;

        out[*count].base = read_cells(reg + i, address_cells);
        out[*count].size = size;
        (*count)++;
    }
}

/* memory@40000000 {
 *   reg = <0x00 0x40000000 0x00 0x8000000>;
 *   device_type = "memory";
 * };
 * */
void fdt_find_memory_regions(void* fdt)
{
    mem_region_count = 0;

    int node = -1;
    while ((node = fdt_node_offset_by_prop_value(fdt, node, "device_type", "memory", sizeof("memory"))) >= 0) {
        int parent = fdt_parent_offset(fdt, node);
        if (parent < 0)
            continue;

        add_memory_region(fdt, node, parent, mem_regions, &mem_region_count);
        if (mem_region_count >= MAX_MEMORY_REGIONS)
            break;
    }

    /* register reserved regions e.g. devicetree, kernel code */


}
