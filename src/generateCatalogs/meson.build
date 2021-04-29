generateCatalogs = executable(
  'generateCatalogs',
  sources             : files('generateCatalogs.c') +
                        generateCatalogs_src +
                        [rasterFontsData_c],
  c_args              : '-DGENERATE_CATALOGS',
  include_directories : [decNumber_inc, wp43s_inc],
  dependencies        : [gtk_dep, gmp_dep],
  native              : true)
softmenuCatalogs_h = custom_target(
  'softmenuCatalogs.h',
  output  : 'softmenuCatalogs.h',
  command : [generateCatalogs, '@OUTPUT@'])
