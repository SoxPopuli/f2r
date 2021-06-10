project('f2r', 'cpp',
  version : '0.1',
  default_options : [
    'warning_level=3',
    'cpp_std=c++20',
    'pkg_config_path=@0@'.format( meson.current_build_dir() )
  ])

cmake = import('cmake')
cmake_opt = cmake.subproject_options()

glbinding = dependency('glbinding', required: true)
fmt = dependency('fmt', required: true)
spdlog = dependency('spdlog', required: true)

sdl2 = dependency('sdl2')
if sdl2.found() == false
  sdl2_proj = cmake.subproject('SDL')
  sdl2 = [ sdl2_proj.dependency('SDL2'), sdl2_proj.dependency('SDL2main') ]
endif

portaudio = dependency('portaudio')
if portaudio.found() == false
  portaudio_proj = cmake.subproject('portaudio')
  portaudio = portaudio_proj.dependency('portaudio')
endif

mp3lame = dependency('libmp3lame', required: false, disabler: true)

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
