libc = static_library('c',
  'string/memchr.c',
  'string/memcmp.c',
  'string/memmove.c',
  'string/memrchr.c',
  'string/strcmp.c',
  'string/strlen.c',
  'string/strncmp.c',
  'string/strnlen.c',
  'string/strrchr.c',
  'string/aarch64/memcpy.S',
  'string/aarch64/memset.S',
  include_directories: libc_inc,
)

libc_dep = declare_dependency(
  link_with: libc,
  include_directories: libc_inc,
)

# Lets subprojects (libfdt) find our libc with dependency('miosb-libc').
meson.override_dependency('miosb-libc', libc_dep)
