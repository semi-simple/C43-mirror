executable(
  'WP43S.elf',
  sources             : decNumber_src + wp43s_src + dmcp_src +
                        [rasterFontsData_c, softmenuCatalogs_h,
                          constantPointers_ch, version_h],
  c_args              : ['-D__weak="__attribute__((weak))"',
                          '-D__packed="__attribute__((__packed__))"',
                          '-Wno-unused-parameter'],
  link_args           : ['-T' + meson.current_source_dir() + '/stm32_program.ld',
                          '--specs=nosys.specs',
                          '-Wl,-Map=' + meson.current_build_dir() + '/WP43S.map,--cref',
                          '-Wl,--gc-sections',
                          '-Wl,--wrap=_malloc_r'],
  include_directories : [decNumber_inc, wp43s_inc, dmcp_inc],
  dependencies        : arm_gmp_dep,
  override_options    : ['debug=false', 'optimization=s'])
