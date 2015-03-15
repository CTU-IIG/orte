/*
 *  $Id: publisher.c,v 0.0.0.1          2003/12/27
 *
 *  DEBUG:  section                     publisher
 *
 *  -------------------------------------------------------------------
 *                                ORTE
 *                      Open Real-Time Ethernet
 *
 *                      Copyright (C) 2001-2006
 *  Department of Control Engineering FEE CTU Prague, Czech Republic
 *                      http://dce.felk.cvut.cz
 *                      http://www.ocera.org
 *
 *  Author:              Petr Smolik	petr@smoliku.cz
 *  Advisor:             Pavel Pisa
 *  Project Responsible: Zdenek Hanzalek
 *  --------------------------------------------------------------------
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 */

#include <stdio.h>
#ifdef __RTL__
  #include <posix/pthread.h>
#endif
#include "orte.h"

#ifdef MAIN_RENAMED
#define main orte_r_publisher_main
#define exit return
#endif

static ORTEDomain        *d;
static char              instance2Send[64];
static int               counter = 0;

static void
sendCallBack(const ORTESendInfo *info, void *vinstance, void *sendCallBackParam)
{
  char *instance = (char *)vinstance;

  switch (info->status) {
    case NEED_DATA:
      printf("Sampling publication, count %d\n", counter);
      sprintf(instance, "Hello Universe! (%d)", counter++);
      break;
    case CQL:  //criticalQueueLevel
      break;
  }
}


int
main(int argc, char *args[])
{
  ORTEPublication *p;
  NtpTime         persistence = NTPTIME_INITIALIZER(3), repeating = NTPTIME_INITIALIZER(1);

  ORTEInit();
  d = ORTEDomainAppCreate(ORTE_DEFAULT_DOMAIN, NULL, NULL, ORTE_FALSE);
  ORTETypeRegisterAdd(d, "HelloMsg", NULL, NULL, NULL, sizeof(instance2Send));
  p = ORTEPublicationCreate(
    d,
    "Reliable HelloMsg",
    "HelloMsg",
    &instance2Send,
    &persistence,
    1,
    sendCallBack,
    NULL,
    &repeating);
  if (p == NULL) {
    printf("ORTEPublicationCreate failed\n");
    return 1;
  }
  #ifndef CONFIG_ORTE_RT
  while (1)
    ORTESleepMs(1000);

  #endif
  return 0;
}

#ifdef CONFIG_ORTE_RT
void
hello_init(void)
{
  main(0, NULL);
}
void
hello_exit(void)
{
  ORTEDomainAppDestroy(d);
}
MODULE_LICENSE("GPL");
module_init(hello_init);
module_exit(hello_exit);
#endif
