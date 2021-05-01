dmcp_cargs = [
  '-D__weak="__attribute__((weak))"',
  '-D__packed="__attribute__((__packed__))"',
  '-Wno-unused-parameter']
dmcp_linkargs = [
  '-T' + meson.current_source_dir() + '/stm32_program.ld',
  '--specs=nosys.specs',
  '-Wl,-Map=' + meson.current_build_dir() + '/WP43S.map,--cref',
  '-Wl,--gc-sections',
  '-Wl,--wrap=_malloc_r']
wp43s_lib = static_library(
  'libwp43s',
  build_by_default    : false,
  sources             : decNumber_src + wp43s_src +
                        [rasterFontsData_c, softmenuCatalogs_h,
                          constantPointers_ch, version_h],
  c_args              : dmcp_cargs,
  link_args           : dmcp_linkargs,
  include_directories : [decNumber_inc, wp43s_inc, dmcp_inc],
  dependencies        : arm_gmp_dep,
  override_options    : ['debug=false', 'optimization=s'])

# First pass building the ELF to get the size of the QSPI
wp43s_pre_elf = executable(
  'WP43S_pre.elf',
  build_by_default    : false,
  sources             : dmcp_src + [version_h],
  c_args              : dmcp_cargs,
  link_args           : dmcp_linkargs,
  link_with           : wp43s_lib,
  include_directories : ['stub_includes', wp43s_inc, dmcp_inc],
  override_options    : ['debug=false', 'optimization=s'])
wp43s_pre_qspi_incorrect_crc = custom_target(
  'WP43S_pre_qspi_incorrect_crc.bin',
  build_by_default : false,
  input            : [wp43s_pre_elf],
  output           : 'WP43S_pre_qspi_incorrect_crc.bin',
  command          : [arm_objcopy, '--only-section', '.qspi', '-O', 'binary', '@INPUT@', '@OUTPUT@'])
wp43s_pre_qspi = custom_target(
  'WP43S_pre_qspi.bin',
  build_by_default : false,
  input            : [wp43s_pre_qspi_incorrect_crc],
  output           : 'WP43S_pre_qspi.bin',
  command          : [modify_crc, forcecrc32, '@INPUT@', '@OUTPUT@'])
qspi_crc_h = custom_target(
  'qspi_crc.h',
  build_by_default : false,
  input            : [wp43s_pre_qspi],
  output           : 'qspi_crc.h',
  command          : [gen_qspi_crc, '@INPUT@', '@OUTPUT@'])

# Second pass building the ELF including the correct qspi_crc.h
wp43s_elf = executable(
  'WP43S.elf',
  build_by_default    : false,
  sources             : dmcp_src + [version_h, qspi_crc_h],
  c_args              : dmcp_cargs,
  link_args           : dmcp_linkargs,
  link_with           : wp43s_lib,
  include_directories : [wp43s_inc, dmcp_inc],
  override_options    : ['debug=false', 'optimization=s'])
wp43s_flash = custom_target(
  'WP43S_flash.bin',
  build_by_default : false,
  input            : [wp43s_elf],
  output           : 'WP43S_flash.bin',
  command          : [arm_objcopy, '--remove-section', '.qspi', '-O', 'binary', '@INPUT@', '@OUTPUT@'])
wp43s_pgm = custom_target(
  'WP43S.pgm',
  build_by_default : false,
  input            : [wp43s_flash],
  output           : 'WP43S.pgm',
  command          : [add_pgm_chsum, '@INPUT@', '@OUTPUT@'])
wp43s_qspi_incorrect_crc = custom_target(
  'WP43S_qspi_incorrect_crc.bin',
  build_by_default : false,
  input            : [wp43s_elf],
  output           : 'WP43S_qspi_incorrect_crc.bin',
  command          : [arm_objcopy, '--only-section', '.qspi', '-O', 'binary', '@INPUT@', '@OUTPUT@'])
wp43s_qspi = custom_target(
  'WP43S_qspi.bin',
  build_by_default : false,
  input            : [wp43s_qspi_incorrect_crc],
  output           : 'WP43S_qspi.bin',
  command          : [modify_crc, forcecrc32, '@INPUT@', '@OUTPUT@'])

alias_target('dmcp', wp43s_pgm, wp43s_qspi)
