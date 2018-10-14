orte_idl = executable(
  'orte-idl',
  [
    'orte-idl-main.c',
    'orte-idl-driver.c',
    'orte-idl-utils.c',
    'orte-idl-c-backend.c',
    'orte-idl-c-headers.c',
    'orte-idl-c-impls.c',
    'orte-idl-c-utils.c',
  ],
  dependencies: [
    dependency('popt'),
    dependency('libIDL-2.0'),
  ],
  install: true,
)

# TODO: Add support for output directory to orte-idl
gen_orte_idl = generator(
  orte_idl,
  output: ['@BASENAME@.c', '@BASENAME@.h'],
  arguments : ['@INPUT@'],
)
