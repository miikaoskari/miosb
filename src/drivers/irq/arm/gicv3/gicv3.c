#include "miosb/driver.h"

int gicv3_probe(device_t *dev)
{
    return 0;
}


void gicv3_driver_init(device_t *dev)
{
    // enables gic cpu -> gicc_ctlr_b group 0
    return;
}

static const char *const gicv3_compat[] = { "arm,gic-v3", NULL };

ASSING_DRIVER driver_t gicv3_driver = {
    .init = gicv3_driver_init,
    .name = "gic-v3",
    .compatible = gicv3_compat,
    .probe = gicv3_probe,
    .type = DRIVER_TYPE_IRQ,
};
