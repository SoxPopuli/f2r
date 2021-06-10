project('f2r', 'cpp',
  version : '0.1',
  default_options : [
    'warning_level=3',
    'cpp_std=c++20'
  ])

glbinding = dependency('glbinding', required: true)
sdl2 = dependency('sdl2', required: true)
fmt = dependency('fmt', required: true)
spdlog = dependency('spdlog', required: true)
portaudio = dependency('portaudiocpp', required: true)


ffmpeg_deps = [
  dependency('libavcodec',    method:  'pkg-config',  required:  true),
  dependency('libavdevice',   method:  'pkg-config',  required:  true),
  dependency('libavfilter',   method:  'pkg-config',  required:  true),
  dependency('libavformat',   method:  'pkg-config',  required:  true),
  dependency('libavutil',     method:  'pkg-config',  required:  true),
]

config_data = configuration_data()
host_endian = host_machine.endian()
if host_endian == 'little' or host_endian == 'big'
  config_data.set('system_endian', 'cfg::endian::' + host_endian)
else
  error('System endian unknown')
endif

config_inc = include_directories('.')
configure_file(
  configuration: config_data, 
  input: 'config.hpp.in',
  output: 'config.hpp')
# build file libraries
subdir('src/file_libs')

if get_option('build_tools')
  cxxopts = dependency('cxxopts', required: true)
  subdir('tools')
endif

inc = include_directories('src')
srcs = files(
  'src/main.cpp',
  'src/window.cpp'
)
executable('f2r',
           srcs,
           include_directories: [ config_inc, inc ],
           dependencies: [ fmt, spdlog, glbinding, sdl2, file_deps ],
           install : true)
