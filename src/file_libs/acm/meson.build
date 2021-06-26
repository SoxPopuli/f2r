acm_inc = include_directories('.')
acm_srcs = files(
  'acm.cpp',
  'wav.cpp'
)

acm_lib = library('acm',
  acm_srcs,
  include_directories: [ acm_inc, config_inc ],
  dependencies: [ common, portaudio ],
  install: true
)

acm = declare_dependency(
  link_with: [acm_lib],
  include_directories: [ file_libs_inc, config_inc ],
  dependencies: [ common, portaudio ]
)
