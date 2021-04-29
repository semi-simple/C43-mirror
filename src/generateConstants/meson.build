generateConstants = executable(
  'generateConstants',
  sources             : files('generateConstants.c') + decNumber_src,
  include_directories : [decNumber_inc, wp43s_inc],
  native              : true)
constantPointers_ch = custom_target(
  'constantPointers.ch',
  output  : ['constantPointers.c', 'constantPointers.h'],
  command : [generateConstants, '@OUTPUT@'])
