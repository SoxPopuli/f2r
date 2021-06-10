#libpng = dependency('libpng', disabler: true)
#oiio = dependency('OpenImageIO', disabler: true)

frm_to_img_srcs = files(
  'stb_image_write.cpp',
  'frm_to_img.cpp'
)
frm_to_img = executable('frm_to_img',
  frm_to_img_srcs,
  dependencies: [ frm, cxxopts ],
  install: true
)
