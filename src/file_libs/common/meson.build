common_inc = include_directories('.') 
common_srcs = files(
  'common.cpp'
)
common_lib = library('common',
  common_srcs,
  include_directories: [common_inc, config_inc],
  install: true
)

common = declare_dependency(
  link_with: [common_lib],
  include_directories: [ file_libs_inc, config_inc ]
)

if get_option('build_tests')
  subdir('tests')
endif
