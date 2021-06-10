pal_inc = include_directories('.')
pal_srcs = files(
  'pal.cpp'
)

pal_lib = library('pal',
  pal_srcs,
  include_directories: [ pal_inc, config_inc ],
  dependencies: [ common ],
  install: true
)

pal = declare_dependency(
  link_with: [pal_lib],
  include_directories: [ file_libs_inc, config_inc ],
  dependencies: [ common ]
)
