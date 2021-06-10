f2_media_convert_srcs = files(
  'f2_media_convert.cpp'
)

f2_media_convert = executable('f2_media_convert',
  f2_media_convert_srcs,
  dependencies: [ ffmpeg_deps, cxxopts, fmt, portaudio ],
  install: true
)
