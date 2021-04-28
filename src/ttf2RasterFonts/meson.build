ttf2RasterFonts = executable(
  'ttf2RasterFonts',
  sources      : files('ttf2RasterFonts.c'),
  dependencies : ft_dep)
rasterFontsData_c = custom_target(
  'rasterFontsData.c',
  output  : 'rasterFontsData.c',
  input   : '../../res/fonts',
  command : [ttf2RasterFonts, '@INPUT@', '@OUTPUT@'])
