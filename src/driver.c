#include "miosb/driver.h"
#include <libfdt.h>
#include <log.h>

const driver_t *find_driver(void *fdt, const int node);


const driver_t *find_driver(void *fdt, const int node)
{
    /* get the count of compatible values */
    int n = fdt_stringlist_count(fdt, node, "compatible");
    for (int i = 0; i < n; i++)
    {
        const char *compatible_for = fdt_stringlist_get(fdt, node, "compatible", i, NULL);

        /* does any of the drivers define any of the compatible strings within
         * the fdt stringlist for the current device */
        FOR_EACH_DRIVER(driver)
        {
            for (const char *const *compatible_driver = driver->compatible; *compatible_driver; compatible_driver++)
            {
                if (strcmp(compatible_for, *compatible_driver) == 0)
                {
                    info("registering driver %s", *compatible_driver);
                    return driver;
                }
            }
        }
    }
    return NULL;
}


void probe_all_drivers_from_fdt(void *fdt)
{
    if (fdt_check_header(fdt) != 0)
    {
        return;
    }

    int node_offset = 0;
    int depth = 0;

    int size_cells = fdt_size_cells(fdt, node_offset);
    int address_cells = fdt_address_cells(fdt, node_offset);

    /* walk through the device tree
     * non-recursive for now */
    while (node_offset >= 0)
    {
        const driver_t *driver = find_driver(fdt, node_offset);

        if (driver)
        {
            int len;

            device_t device = {0};

            device.name = fdt_get_name(fdt, node_offset, NULL);

            const fdt32_t *reg = fdt_getprop(fdt, node_offset, "reg", &len);

            if (reg && len > 0)
            {
                /* we have high 32 bits at location 0 and low 32 bits at location 1 */

                uint64_t addr = 0;
                for (int i = 0; i < address_cells; i++)
                {
                    addr = (addr << 32) | fdt32_to_cpu(reg[i]);
                }
                // device.reg_base = addr;

                uint64_t size = 0;
                for (int i = 0; i < size_cells; i++)
                {
                    size = (size << 32) | fdt32_to_cpu(reg[address_cells + i]);
                }
                // device.reg_size = size;
            }

            driver->init(&device);
        }

        node_offset = fdt_next_node(fdt, node_offset, &depth);
    }
}

