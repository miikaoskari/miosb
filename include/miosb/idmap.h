#ifndef IDMAP_H
#define IDMAP_H

#include "miosb/platform/aarch64/pgtable.h"
#include "miosb/types.h"
#include <stdbool.h>

/* Maps [start, end) into the identity map, growing the page-table pool as needed. */
void idmap_add_region(u64 start, u64 end, unsigned attridx, bool exec);

void init_idmap(void);

#endif // IDMAP_H
