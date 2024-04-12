cc = meson.get_compiler('c')

lib_deps = [
  dependency('glew'),
  dependency('sdl2'),
  cc.find_library('m', required: false),
]

srcs = files([
  'cg_core.c',
  'cg_gfx.c',
  'cg_input.c',
  'cg_math.c',
  'cg_util.c',
])

cg_lib_build = library(
  'cg',
  sources: srcs,
  install: true,
  dependencies: [lib_deps, declare_dependency(sources: resources)],
  include_directories: incdir
)

cg_lib = declare_dependency(link_with: cg_lib_build, include_directories: incdir)

cg_deps = lib_deps + cg_lib
