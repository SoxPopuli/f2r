mve_inc = include_directories('.')
mve_srcs = files(
  'mve.cpp',
)

mve_lib = library('mve',
  mve_srcs,
  include_directories: [ mve_inc, config_inc ],
  dependencies: [ common ],
  install: true
)

mve = declare_dependency(
  link_with: [mve_lib],
  include_directories: [ file_libs_inc, config_inc ],
  dependencies: [ common ]
)
