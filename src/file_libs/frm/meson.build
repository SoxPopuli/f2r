frm_inc = include_directories('.')
frm_srcs = files(
  'frm.cpp'
)

frm_lib = library('frm',
  frm_srcs,
  include_directories: [ frm_inc, config_inc ],
  dependencies: [ pal, common ],
  install: true
)

frm = declare_dependency(
  link_with: [frm_lib],
  include_directories: [ file_libs_inc, config_inc ],
  dependencies: [ pal, common ]
)
