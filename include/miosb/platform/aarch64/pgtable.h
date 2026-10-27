#ifndef PGTABLE_H
#define PGTABLE_H

#include "miosb/bits.h"
#include "miosb/types.h"
#include <stdbool.h>

#define PTE_TYPE_MASK GENMASK(1, 0)
#define PTE_TYPE_TABLE UL(0b11)
#define PTE_TYPE_BLOCK UL(0b01)
#define PTE_TYPE_PAGE UL(0b11) /* level-3 page descriptor; shares its encoding with PTE_TYPE_TABLE */

/* D4.3.3 Memory attribute fields in the VMSAv8-64 translation table format descriptors */

#define PTE_XN_OR_UXN BIT(54)
#define PTE_PXN BIT(53)
#define PTE_NG BIT(11)
#define PTE_AF BIT(10)
#define PTE_SH_MASK GENMASK(9, 8)
#define PTE_SH_INNER (UL(0b11) << 8)
#define PTE_AP GENMASK(7, 6)
#define PTE_NS BIT(5)
//#define PTE_ATTRINDX GENMASK(4, 2)
#define PTE_ATTRINDX_SHIFT  2
#define PTE_ATTRINDX(idx)   ((idx) << PTE_ATTRINDX_SHIFT)

#define MT_NORMAL 0 /* MAIR index 0 */
#define MT_DEVICE 1 /* MAIR index 1 */

static inline u64 pte_table(u64 pa)
{
    return pa | PTE_TYPE_TABLE;
}

static inline u64 pte_block(u64 pa, unsigned attridx, bool exec)
{
    u64 d = pa | PTE_TYPE_BLOCK | PTE_AF | PTE_SH_INNER | PTE_ATTRINDX(attridx);
    if (!exec)
        d |= PTE_XN_OR_UXN | PTE_PXN;
    return d;
}

static inline u64 pte_page(u64 pa, unsigned attridx, bool exec)
{
    u64 d = pa | PTE_TYPE_PAGE | PTE_AF | PTE_SH_INNER | PTE_ATTRINDX(attridx);
    if (!exec)
        d |= PTE_XN_OR_UXN | PTE_PXN;
    return d;
}

#endif // PGTABLE_H
