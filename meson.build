project('wp43s', 'c')

add_global_arguments('-DOSX', language : 'c')
add_global_arguments('-DPC_BUILD', language : 'c')
add_global_arguments('-DOS64BIT', language : 'c')

cc = meson.get_compiler('c')

ft_dep  = dependency('freetype2')
gtk_dep = dependency('gtk+-3.0')
m_dep   = cc.find_library('m', required : false)

conf_data = configuration_data()
conf_data.set('CI_COMMIT_TAG', '0.1.20')
conf_data.set('TODAY', '2021.05.01')
conf_data.set('GIT_SHA', 'abcdef88')

subdir('docs/code')
subdir('dep')
subdir('src/wp43s')
subdir('src/ttf2RasterFonts')
subdir('src/generateConstants')
subdir('src/generateCatalogs')
subdir('src/generateTestPgms')
subdir('src/testSuite')
subdir('src/wp43s-gtk')
