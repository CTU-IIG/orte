install_headers(
  'basic_types.h',
  'cdr.h',
  'defines_api.h',
  'protos_api.h',
  'typedefs_api.h',
  'typedefs_defines_rtps.h',
  subdir: 'orte',
)

if (host_machine.system() == 'windows')
  orte_config_src = 'orte_config_omk_win32.h'
elif (host_machine.system() == 'linux')
  orte_config_src = 'orte_config_omk_linux.h'
else
  orte_config_src = 'orte_config_omk_linux.h'
endif

orte_config_h = custom_target(
  'orte_config.h',
  input : orte_config_src,
  output : 'orte_config.h',
  command : ['cp', '@INPUT@', '@OUTPUT@'],
  install: true,
  install_dir: 'include/orte',
)
