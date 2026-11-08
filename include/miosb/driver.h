#ifndef DRIVER_H
#define DRIVER_H

#include <libfdt.h>

#define MAX_DRIVERS 64
#define MAX_DEVICE_REGIONS 8
#define ASSING_DRIVER __attribute__((used, section(".drivers")))

struct device_s;
struct driver_s;

typedef enum device_type_enum {
    DRIVER_TYPE_SERIAL,
    DRIVER_TYPE_IRQ,
    DRIVER_TYPE_BLOCK,
} device_type_t;

typedef struct driver_s
{
    void (*init)(struct device_s *dev);
    const char *name;
    const char *const *compatible; /* must be null terminated */

    int (*probe)(struct device_s *dev);

    device_type_t type;
} driver_t;

extern driver_t __drivers_start;
extern driver_t __drivers_end;

#define FOR_EACH_DRIVER(driver) \
    for (driver_t *(driver) = &(__drivers_start); driver < &(__drivers_end); ++driver)

typedef struct device_s
{
    const char *name;
    const char *compatible;

    uintptr_t reg_base[MAX_DEVICE_REGIONS];
    uintptr_t reg_size[MAX_DEVICE_REGIONS];
    int region_count;

    int irq;

    void *data;

    const driver_t *driver;
} device_t;

void register_all_drivers(void);
void probe_all_drivers_from_fdt(void *fdt);
void register_new_driver(const driver_t *driver);

#endif // DRIVER_H
