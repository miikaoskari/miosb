project('libfdt', 'c',
  version: '1.7.2',
  default_options: ['warning_level=0'],
)

libc_dep = dependency('miosb-libc')
libfdt_inc = include_directories('libfdt', is_system: true)

libfdt = static_library('fdt',
  'libfdt/fdt.c',
  'libfdt/fdt_addresses.c',
  'libfdt/fdt_ro.c',
  'libfdt/fdt_wip.c',
  'libfdt/fdt_sw.c',
  'libfdt/fdt_rw.c',
  'libfdt/fdt_strerror.c',
  include_directories: libfdt_inc,
  dependencies: libc_dep,
)

libfdt_dep = declare_dependency(
  link_with: libfdt,
  include_directories: libfdt_inc,
  dependencies: libc_dep,
)
