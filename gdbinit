# GDB initialization script for kernel debugging
set confirm off
set print pretty on
set print array on
set print object on
set print static-members on
set print vtbl on
set print demangle on
set demangle-style gnu-v3
set print sevenbit-strings off

# Connect to QEMU GDB server
target remote localhost:1234

# Load symbols from kernel
file build/kernel.elf

# Set architecture
set architecture aarch64

# Break on kernel main
break kmain

# Add reset helper
define reset
    monitor system_reset
    echo [qemu reset]\n
end

define rrr
    echo [reset + reload + run]\n
    monitor system_reset
    file build/kernel.elf
    load
    break kmain
    continue
end

define kexit
    kill
    exit
end
