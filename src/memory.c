#include "miosb/memory.h"
#include "miosb/string.h"
#include "miosb/types.h"

/**
 * | order | size    |
 * |-------|---------|
 * | 0     | 4kb     |
 * | 1     | 8kb     |
 * | 2     | 16kb    |
 * | 3     | 32kb    |
 * | 4     | 64kb    |
 * | 5     | 128kb   |
 * | 6     | 256kb   |
 * | 7     | 512kb   |
 * | 8     | 1024kb  |
 * | 9     | 2048kb  |
 * | 10    | 4096kb  |
 * | 11    | 8192kb  |
 */

// mem_map uses some random number. should be ram size / page size
struct page mem_map[4096];

// free_list[x] contains all free order x
struct page *free_list[MAX_ORDER + 1];

void phys_mem_init(u64 phys_ram_start, u64 phys_ram_size, u64 kernel_start_addr, u64 kernel_end_addr)
{
    memset(mem_map, 0, sizeof(mem_map));
    memset(free_list, 0, sizeof(free_list));

    struct page *initial_block = mem_map;
    initial_block->order = MAX_ORDER;
    initial_block->next = NULL;

    free_list[MAX_ORDER] = initial_block;

    return;
}

struct page* alloc_pages(const unsigned int order)
{
    if (order > MAX_ORDER) {
        return nullptr;
    }

    struct page *free_block = nullptr;
    unsigned int current_order = order;

    // find free block if any are available
    while (current_order <= MAX_ORDER)
    {
        if (free_list[current_order] != NULL)
        {
            free_block = free_list[current_order];
            free_list[current_order] = free_block->next;
            break;
        }
        current_order++;
    }

    // no block no memory
    if (free_block == NULL)
    {
        return nullptr;
    }

    // split block until desired order is reached
    // travels downwards from bigger order
    while (current_order > order)
    {
        current_order--;

        struct page *buddy = free_block + (1 << current_order);

        buddy->order = current_order;
        buddy->next = free_list[current_order];
        free_list[current_order] = buddy;
    }

    free_block->order = order;
    free_block->next = nullptr;
    return free_block;
}

void free_pages(struct page* page)
{
    if (page == NULL) return;

    page->next = free_list[page->order];
    free_list[page->order] = page;
}
