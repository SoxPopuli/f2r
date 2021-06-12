f2_media_convert_srcs = files(
  'f2_media_convert.cpp'
)

f2_media_convert = executable('f2_media_convert',
  f2_media_convert_srcs,
  dependencies: [ acm, mp3lame, cxxopts, fmt, portaudio ],
  install: true
)
