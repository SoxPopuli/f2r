project('f2r', 'cpp',
  version : '0.1',
  default_options : [
    'warning_level=3',
    'cpp_std=c++20',
    'pkg_config_path=@0@'.format( meson.current_build_dir() )
  ])

pch = '@0@/pch.hpp'.format( meson.current_source_dir() )

cmake = import('cmake')
cmake_opt = cmake.subproject_options()

glbinding = dependency('glbinding', required: true)
fmt = dependency('fmt', required: true)
spdlog = dependency('spdlog', required: true)

sdl2 = dependency('sdl2', required: true)

portaudio = dependency('portaudio-2.0', required: true)

mp3lame = dependency('libmp3lame', required: false, disabler: true)

config_data = configuration_data()
host_endian = host_machine.endian()
if host_endian == 'little' or host_endian == 'big'
  config_data.set('system_endian', 'cfg::endian::' + host_endian)
else
  error('System endian unknown')
endif

config_inc = [ 
  include_directories('.'), 
  include_directories('include') 
]
configure_file(
  configuration: config_data, 
  input: 'config.hpp.in',
  output: 'config.hpp')
# build file libraries

if get_option('build_tools')
  cxxopts = dependency('cxxopts', required: true)
  subdir('tools')
endif

inc = include_directories('include')
subdir('src')

deps = [
  fmt, 
  spdlog,
  glbinding,
  sdl2,
]

core_lib = static_library('core',
  srcs,
  include_directories: [ config_inc, inc ],
  dependencies: deps,
  cpp_pch: pch,
)

core = declare_dependency(
  include_directories: [config_inc, inc],
  dependencies: deps,
  link_with: core_lib,
)

if get_option('build_tests')
  subdir('tests')
endif

executable('f2r',
           files('src/main.cpp'),
           include_directories: [ config_inc, inc ],
           dependencies: core,
           cpp_pch: pch,
           install : true)
