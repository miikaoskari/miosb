#include "miosb/bits.h"
#include "miosb/idmap.h"
#include "miosb/platform/aarch64/mmu.h"
#include "miosb/platform/aarch64/pgtable.h"
#include "miosb/platform/aarch64/sysreg.h"
#include "miosb/string.h"
#include "miosb/types.h"
#include <platform/aarch64/barrier.h>
#include <stdbool.h>

#define GRANULE_SIZE 4096UL
#define TABLE_ENTRIES_COUNT 512

#define PAGE_SHIFT 12
#define TABLE_SHIFT 9
#define PTRS_PER_TABLE (1 << TABLE_SHIFT)

#define L1_SHIFT (PAGE_SHIFT + 2 * TABLE_SHIFT) /* 30 — 1GB per entry */
#define L2_SHIFT (PAGE_SHIFT + TABLE_SHIFT)     /* 21 — 2MB per entry */
#define L3_SHIFT PAGE_SHIFT                     /* 12 — 4KB per entry */

#define L1_IDX(va) (((va) >> L1_SHIFT) & (TABLE_ENTRIES_COUNT - 1))
#define L2_IDX(va) (((va) >> L2_SHIFT) & (TABLE_ENTRIES_COUNT - 1))
#define L3_IDX(va) (((va) >> L3_SHIFT) & (TABLE_ENTRIES_COUNT - 1))

#define ALIGN_DOWN(x, a) ((x) & ~((a) - 1))
#define ALIGN_UP(x, a) (((x) + (a) - 1) & ~((a) - 1))

#define PTE_ADDR_MASK (~(u64)(GRANULE_SIZE - 1))

extern char __kernel_start[];
extern char __kernel_end[];

/*
 * Single flat pool that every level of the identity map is carved out of,
 * bump-allocated one table at a time (cf. Linux's init_pg_dir). The first
 * table handed out becomes the L1 root; walking down to 4KB pulls further
 * L2/L3 tables from the same pool as they're needed.
 */
#define PGTABLE_POOL_TABLES 32
__attribute__((aligned(GRANULE_SIZE))) static u64 pgtable_pool[PGTABLE_POOL_TABLES][TABLE_ENTRIES_COUNT];
static unsigned pgtable_pool_next = 0;

static u64 *l1_table = nullptr;

static u64 *pgtable_alloc(void)
{
    if (pgtable_pool_next >= PGTABLE_POOL_TABLES)
    {
        /* Pool exhausted this early in boot — nothing to do but stop. */
        for (;;)
            wfi();
    }

    u64 *table = pgtable_pool[pgtable_pool_next++];
    memset(table, 0, GRANULE_SIZE);
    return table;
}

/* Returns the next-level table `entry` points to, allocating and linking one from the pool if absent. */
static u64 *table_walk(u64 *entry)
{
    if ((*entry & PTE_TYPE_MASK) != PTE_TYPE_TABLE)
    {
        u64 *table = pgtable_alloc();
        *entry = pte_table((u64)table);
    }

    return (u64 *)(*entry & PTE_ADDR_MASK);
}

static void enable_mmu(u64 t0sz, u64 ips)
{
    u64 tcr_val = 0;
    tcr_val |= FIELD_PREP(TCR_EL1_T0SZ_MASK, t0sz);
    tcr_val |= FIELD_PREP(TCR_EL1_TG0_MASK, TCR_EL1_TG0_4K);
    tcr_val |= FIELD_PREP(TCR_EL1_SH0_MASK, TCR_EL1_SH0_INNER);
    tcr_val |= FIELD_PREP(TCR_EL1_ORGN0_MASK, TCR_EL1_RGN_WBWA);
    tcr_val |= FIELD_PREP(TCR_EL1_IRGN0_MASK, TCR_EL1_RGN_WBWA);
    tcr_val |= FIELD_PREP(TCR_EL1_IPS_MASK, ips);
    tcr_val |= TCR_EL1_EPD1;

    write_sysreg(tcr_val, tcr_el1);

    write_sysreg(MAIR_EL1_ATTRIDX(MAIR_EL1_ATTR_NORMAL, MT_NORMAL) |
                 MAIR_EL1_ATTRIDX(MAIR_EL1_ATTR_DEVICE_nGnRnE, MT_DEVICE),
                 mair_el1);

    write_sysreg((u64)l1_table, ttbr0_el1);

    u64 sctlr_val = 0;
    sctlr_val |= SCTLR_EL1_M;

    write_sysreg(sctlr_val, sctlr_el1);

    asm volatile("tlbi vmalle1" : : : "memory");
    dsb(ish);
    isb();

    return;
}

static u64 read_aa64mmfr0_parange(void)
{
    u64 mmfr0 = read_sysreg(id_aa64mmfr0_el1);
    u64 parange = FIELD_GET(ID_AA64MMFR0_EL1_PARANGE_MASK, mmfr0);
    return parange;
}

void idmap_add_region(u64 start, u64 end, unsigned attridx, bool exec)
{
    u64 region_start = ALIGN_DOWN(start, GRANULE_SIZE);
    u64 region_end = ALIGN_UP(end, GRANULE_SIZE);

    if (l1_table == nullptr)
    {
        l1_table = pgtable_alloc();
    }

    for (u64 pa = region_start; pa < region_end; pa += GRANULE_SIZE)
    {
        u64 *l2_table = table_walk(&l1_table[L1_IDX(pa)]);
        u64 *l3_table = table_walk(&l2_table[L2_IDX(pa)]);
        l3_table[L3_IDX(pa)] = pte_page(pa, attridx, exec);
    }

    return;
}

void init_idmap(void)
{
    idmap_add_region((uptr)__kernel_start, (uptr)__kernel_end, MT_NORMAL, true);

    u64 parange = read_aa64mmfr0_parange();

    enable_mmu(25, parange);

    return;
}
