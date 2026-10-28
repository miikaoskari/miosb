project('miosb', 'c',
  meson_version: '>=1.4.0',
  default_options: [
    'c_std=gnu23',
    'buildtype=debug',
    'warning_level=3',
    'b_staticpic=false',
  ],
)

# headers for our own small libc used in place of
# the system ones by libc itself, libfdt and the kernel
libc_inc = include_directories('include/miosb', is_system: true)
kernel_inc = include_directories('include')

subdir('lib')

libfdt_dep = subproject('dtc').get_variable('libfdt_dep')
malgos_dep = dependency('malgos')

subdir('src')

linker_script = meson.project_source_root() / 'linker.ld'

kernel = executable('kernel', kernel_sources,
  name_suffix: 'elf',
  include_directories: kernel_inc,
  dependencies: [libfdt_dep, malgos_dep, libc_dep],
  c_args: [
    '-fno-omit-frame-pointer',
    '-Wconversion',
    '-Wsign-conversion',
    '-Wshadow',
    '-Wpointer-arith',
    '-Wcast-align',
    '-Wwrite-strings',
    '-Wmissing-prototypes',
    '-Wstrict-prototypes',
    '-Wno-language-extension-token'
  ],
  link_args: ['-T', linker_script],
  link_depends: linker_script,
)

objcopy = find_program('objcopy')

custom_target('kernel.img',
  input: kernel,
  output: 'kernel.img',
  command: [objcopy, '-O', 'binary', '@INPUT@', '@OUTPUT@'],
  build_by_default: true,
)
