/*  Init
 *
 *  This routine is the initialization task for this test program.
 *  It is called from init_exec and has the responsibility for creating
 *  and starting the tasks that make up the test.  If the time of day
 *  clock is required for the test, it should also be set to a known
 *  value by this function.
 *
 *  Input parameters:  NONE
 *
 *  Output parameters:  NONE
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: init.c,v 1.12.4.1 2003/09/04 18:46:30 joel Exp $
 */

#define CONFIGURE_INIT
#include "rtems_shell_local_config.h"
#include "system.h"
#include "app_def.h"
#include <stdio.h>
#include <stdlib.h>
#include <rtems/error.h>
#include <rtems/monitor.h>
#include <rtems/shell.h>

#include <rtems/rtems_bsdnet.h>

#include <orte_rtems_shell.h>

#include "networkconfig-static.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define CONFIGURE_SHELL_COMMANDS_INIT
#define CONFIGURE_SHELL_COMMANDS_ALL
#define CONFIGURE_SHELL_COMMANDS_ALL_NETWORKING
#define CONFIGURE_SHELL_MOUNT_MSDOS
#define CONFIGURE_SHELL_MOUNT_NFS

#include <rtems/shellconfig.h>

#define BUILD_VERSION_STRING(major,minor,patch) \
        __XSTRING(major) "." __XSTRING(minor) "." __XSTRING(patch)

#ifndef VER_CODE
  #define VER_CODE(major,minor,patch) (major*0x10000+minor*0x100+patch)
#endif

#define RTEMS_VER_CODE VER_CODE(__RTEMS_MAJOR__ ,__RTEMS_MINOR__ ,__RTEMS_REVISION__)

#if RTEMS_VER_CODE < VER_CODE(4,7,99)
  #define rtems_shell_add_cmd shell_add_cmd
  #define rtems_shell_init(m_task_name,m_task_stacksize,m_task_priority,m_devname,m_forever,m_wait,m_login_check) \
                shell_init(m_task_name,m_task_stacksize,m_task_priority,m_devname,B19200 | CS8,m_forever)
#elif RTEMS_VER_CODE < VER_CODE(4,9,99)
  #define rtems_shell_init(m_task_name,m_task_stacksize,m_task_priority,m_devname,m_forever,m_wait,m_login_check) \
          rtems_shell_init(m_task_name,m_task_stacksize,m_task_priority,m_devname,m_forever,m_wait)
#endif

void
bad_rtems_status(rtems_status_code status, int fail_level, const char *text)
{
  printf("ERROR: %s status %s", text, rtems_status_text(status));
  status = rtems_task_delete( RTEMS_SELF );
}

int testcmd_forshell(int argc, char **argv)
{
  int i;
  printf("Command %s called\n",argv[0]);
  for(i=1;i<argc;i++)
    if(argv[i])
      printf("%s",argv[i]);
  printf("\n");
  return 0;
}

rtems_task Init(
  rtems_task_argument ignored
)
{
  rtems_status_code status;

  printf( "\n\nRTEMS v "
          BUILD_VERSION_STRING(__RTEMS_MAJOR__ ,__RTEMS_MINOR__ ,__RTEMS_REVISION__)
	  "\n");

  rtems_monitor_init(RTEMS_MONITOR_SUSPEND|RTEMS_MONITOR_GLOBAL);
  /*rtems_capture_cli_init (0);*/

  if (rtems_bsdnet_initialize_network() < 0)
    printf( "Network initialization failed\n");
  else
    printf( "Network initialization OK\n");


  printf( "Starting shell with ORTE commands included\n" );

  rtems_shell_init("SHLL",RTEMS_MINIMUM_STACK_SIZE+0x1000,
              SHELL_TASK_PRIORITY,"/dev/console",1,0, NULL);

  rtems_shell_add_cmd("testcmd", "app",
                "test command for shell",
                testcmd_forshell);

 #ifdef CONFIG_OC_ETH_ORTE_MANAGER
  rtems_shell_add_cmd("ortemanager", "orte",
                "start orte manager",
                ortemanager_main);
 #endif /*CONFIG_OC_ETH_ORTE_MANAGER*/

 #ifdef CONFIG_OC_ETH_ORTE_EXAMPLES
  rtems_shell_add_cmd("orte_h_publisher", "orte",
                "start orte_h_publisher",
                orte_h_publisher_main);

  rtems_shell_add_cmd("orte_h_subscriber", "orte",
                "start orte_h_subscriber",
                orte_h_subscriber_main);

  rtems_shell_add_cmd("orte_m_subscriber", "orte",
                "start orte_m_subscriber",
                orte_m_subscriber_main);

  rtems_shell_add_cmd("orteping", "orte",
                "start orteping",
                orte_ping_main);

  rtems_shell_add_cmd("orte_spy", "orte",
                "start orte_spy",
                orte_spy_main);

  rtems_shell_add_cmd("orte_r_publisher", "orte",
                "start orte_r_publisher",
                orte_r_publisher_main);

  rtems_shell_add_cmd("orte_r_subscriber_besteffort", "orte",
                "start orte_r_subscriber_besteffort",
                orte_r_subscriber_besteffort_main);

  rtems_shell_add_cmd("orte_r_subscriber_reliable", "orte",
                "start orte_r_subscriber_reliable",
                orte_r_subscriber_reliable_main);
 #endif /*CONFIG_OC_ETH_ORTE_EXAMPLES*/

  rtems_shell_add_cmd("spawn", "orte",
                "spawn task or command in background",
                orte_spawn_main);

  //rtems_monitor_wakeup();

  status = rtems_task_delete( RTEMS_SELF );

  printf( "*** END OF TEST2 ***\n" );
  exit( 0 );
}
