ttf2RasterFonts = executable(
  'ttf2RasterFonts',
  sources      : files('ttf2RasterFonts.c'),
  dependencies : ft_dep)
rasterFontsData_c = custom_target(
  'rasterFontsData.c',
  output  : 'rasterFontsData.c',
  command : [ttf2RasterFonts, '@CURRENT_SOURCE_DIR@/../../res/fonts', '@OUTPUT@'])
