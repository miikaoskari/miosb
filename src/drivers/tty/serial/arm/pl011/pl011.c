#include "miosb/driver.h"

int pl011_probe(device_t* dev)
{
    return 0;
}

void pl011_driver_init(device_t* dev)
{
    return;
}

static const char *const pl011_compat[] = { "arm,pl011", NULL };

ASSING_DRIVER driver_t pl011_driver = {
    .init = pl011_driver_init,
    .name = "pl011",
    .compatible = pl011_compat,
    .probe = pl011_probe,
    .type = DRIVER_TYPE_SERIAL,
};
