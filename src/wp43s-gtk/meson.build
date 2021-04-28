executable(
  'wp43s',
  sources             : decNumber_src + wp43s_src + files('wp43s-gtk.c') +
                        [rasterFontsData_c, softmenuCatalogs_h,
                          constantPointers_ch, version_h],
  include_directories : [decNumber_inc, wp43s_inc, '/opt/homebrew/include'],
  dependencies        : [gtk_dep, gmp_dep])
