#include "miosb/platform/aarch64/barrier.h"
#include "miosb/types.h"

typedef struct
{
    u64 regs[31];
    u64 __pad; /* x30 is stored alone, see entry.S */
    u64 elr;
    u64 spsr;
    u64 sp;
} pt_regs;

void el1t_64_sync_handler(pt_regs *regs)
{
    wfi();
}

void el1t_64_irq_handler(pt_regs *regs)
{
    wfi();
}

void el1t_64_fiq_handler(pt_regs *regs)
{
    wfi();
}

void el1t_64_error_handler(pt_regs *regs)
{
    wfi();
}

void el1h_64_sync_handler(pt_regs *regs)
{
    wfi();
}

void el1h_64_irq_handler(pt_regs *regs)
{

    wfi();
}

void el1h_64_fiq_handler(pt_regs *regs)
{
    wfi();
}

void el1h_64_error_handler(pt_regs *regs)
{
    wfi();
}

void el0t_64_sync_handler(pt_regs *regs)
{
    wfi();
}

void el0t_64_irq_handler(pt_regs *regs)
{
    wfi();
}

void el0t_64_fiq_handler(pt_regs *regs)
{
    wfi();
}

void el0t_64_error_handler(pt_regs *regs)
{
    wfi();
}

void el0t_32_sync_handler(pt_regs *regs)
{
    wfi();
}

void el0t_32_irq_handler(pt_regs *regs)
{
    wfi();
}

void el0t_32_fiq_handler(pt_regs *regs)
{
    wfi();
}

void el0t_32_error_handler(pt_regs *regs)
{
    wfi();
}
