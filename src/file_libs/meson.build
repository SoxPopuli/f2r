file_libs_inc = include_directories('.')

subdir('common')
subdir('pal')
subdir('frm')
subdir('acm')
subdir('mve')

file_deps = declare_dependency(
  dependencies: [ common, pal, frm, acm, mve ]
)
