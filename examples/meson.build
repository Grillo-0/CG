resources_files = files([
  'resources/suzzanne.mtl',
  'resources/suzzanne.obj',
  'resources/suzzanne_tex.png',
  'resources/suzzanne_left.png',
])

resources = custom_target(
  'gen_resources',
  input : resources_files,
  output : 'resources.c',
  capture: true,
  command : [bed, '@INPUT@'],
)

examples = [
  'camera_fps',
  'cube',
  'obj_loading',
  'triangle',
  'triangle_input',
  'triangle_transform',
]

foreach e : examples
  executable(e, e + '.c', dependencies: [ cg_deps, declare_dependency(sources: resources)]
  )
endforeach
