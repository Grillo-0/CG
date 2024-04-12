resources_files = [
  'shaders/vert.glsl',
  'shaders/frag.glsl',
]

bed = executable('bed', '../src/external/bed.c')

resources = custom_target(
  'gen_resources',
  input : resources_files,
  output : 'resources.c',
  capture: true,
  command : [bed, '-p', 'cg', '@INPUT@'],
)
