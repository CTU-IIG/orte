#include "rtems_shell_local_config.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <pthread.h>
#include <string.h>
#include <getopt.h>
#include <rtems/error.h>
#include <rtems/monitor.h>
#include <rtems/shell.h>

#include <orte_rtems_shell.h>

typedef struct orte_spawn_args {
  int argc;
  char *argv[];
} orte_spawn_args_t;

void *orte_spawn_task_func(void *arg_pack)
{
  rtems_shell_cmd_t* shell_cmd;
  int                errorlevel = 0;
  orte_spawn_args_t  *spawn_args = (orte_spawn_args_t*)arg_pack;
  int                argc = spawn_args->argc;
  char               **argv = &(spawn_args->argv[0]);

  if (optind)
    optind = 1;

  if (argc) {
    shell_cmd = rtems_shell_lookup_cmd(argv[0]);
    if ( argv[1] == NULL ) {
      errorlevel = -1;
    } else if ( shell_cmd == NULL ) {
      errorlevel = rtems_shell_script_file(argc, &argv[0]);
    } else {
      errorlevel = shell_cmd->command(argc, &argv[0]);
    }
  }

  free(arg_pack);
  return (void*)errorlevel;
}

int orte_spawn_main(int argc, char **argv)
{
  orte_spawn_args_t  *spawn_args;
  int       sparg_size;
  int       sparg_str_offs;
  int       status;
  pthread_t task_id;
  int       i;
  char      *p;

  argv++;
  argc--;

  sparg_str_offs = sizeof(orte_spawn_args_t) + (argc + 1) *sizeof(char*);
  sparg_size = sparg_str_offs;

  for (i = 0; i < argc; i++)
     sparg_size += strlen(argv[i]) + 1;

  spawn_args = malloc(sparg_size);
  if (spawn_args == NULL)
     return 1;

  spawn_args->argc = argc;

  p = (char*)spawn_args + sparg_str_offs;
  for (i = 0; i < argc; i++) {
     int len = strlen(argv[i]);
     spawn_args->argv[i] = p;
     memcpy(p, argv[i], len);
     p += len;
     *(p++) = 0;
  }
  spawn_args->argv[argc] = NULL;

  status = pthread_create(&task_id, NULL, orte_spawn_task_func, spawn_args);
  if (status == 0)
    pthread_detach(task_id);

  return status?1:0;
}
