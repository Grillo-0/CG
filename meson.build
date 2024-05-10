project(
  'cg',
  'c',
  default_options: [
    'c_std=gnu11',
    'warning_level=3',
    'buildtype=debugoptimized',
  ],
  version: '0.0',
  license: 'MPL-2.0'
)

incdir = include_directories('include')

bed = executable('bed', 'src/external/bed.c')

subdir('resources')
subdir('include')
subdir('src')
subdir('examples')
