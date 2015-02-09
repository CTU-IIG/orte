#ifndef _APP_DEF_H
#define _APP_DEF_H

#ifndef COND_EXTERN
  #ifdef CONFIGURE_INIT
    #define COND_EXTERN
  #else
    #define COND_EXTERN extern
  #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

void
bad_rtems_status(rtems_status_code status, int fail_level, const char *text);

static inline
void
check_rtems_status(rtems_status_code status, int fail_level, const char *text)
{
  if (!rtems_is_status_successful(status))
    bad_rtems_status(status, fail_level, text);
}

#define TASK_1_PRIORITY     30
#define SHELL_TASK_PRIORITY 50

#ifdef __cplusplus
}
#endif

#endif /*_APP_DEF_H*/
