/*
 *  $Id: HelloWorldPublisher.c,v 0.0.0.1 2003/12/26
 *
 *  DEBUG:  section                     HelloWorldPublisher
 *  AUTHOR: Petr Smolik                 petr.smolik@wo.cz
 *
 *  ORTE - OCERA Real-Time Ethernet     http://www.ocera.org/
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

#ifdef HAVE_CONFIG_H
  #include <orte_config.h>
#endif
#if defined(CONFIG_ORTE_RTL)
  #include <stdio.h>
  #include <posix/pthread.h>
  #define printf rtl_printf
#elif defined(CONFIG_ORTE_RTAI)
  #include <linux/module.h>
  #include <rtai.h>
  #include <rtai_sched.h>
  #include <rtai_sem.h>
  #define printf rt_printk
#else
  #include <stdio.h>
#endif
#include "orte_api.h"

ORTEDomain        *d;
char              instance2Send[64];
char              terminate=0;
int               counter=0;
#ifdef CONFIG_ORTE_RTAI
int               terminated = 0;
SEM               term_sem;
#endif

void
sendCallBack(const ORTESendInfo *info,void *vinstance, void *sendCallBackParam) {
  char *instance=(char*)vinstance;

  switch (info->status) {
    case NEED_DATA:
      printf("Sampling publication, count %d\n", counter);
      sprintf(instance,"Hello Universe! (%d)",counter++);
      break;
    case CQL:  //criticalQueueLevel
      break;
  }
}

void *
publisher(void *arg) {
  ORTEPublication     *p;
  ORTEDomainProp      dp;
  NtpTime             persistence, delay;

  ORTEInit();
//  ORTEVerbositySetOptions("ALL,5");
  ORTEDomainPropDefaultGet(&dp);
  #ifdef CONFIG_ORTE_RTL
  dp.appLocalManager=StringToIPAddress("192.168.4.1");
  #endif
  d=ORTEDomainAppCreate(ORTE_DEFAULT_DOMAIN,&dp,NULL,ORTE_FALSE);
  if (!d) {
    printf("ORTEDomainAppCreate failed!\n");
    return 0;
  }
  ORTETypeRegisterAdd(d,"HelloMsg",NULL,NULL,64);
  NTPTIME_BUILD(persistence,3);
  NTPTIME_BUILD(delay,10); //10s
  p=ORTEPublicationCreate(
       d,
      "Example HelloMsg",
      "HelloMsg",
      &instance2Send,
      &persistence,
      1,
      sendCallBack,
      NULL,
      &delay);
  return 0;
}

#ifndef CONFIG_ORTE_RT
int
main(int argc, char *args[]) {
  publisher(NULL);
  while(1) {
    ORTESleepMs(1000);
  }
  return 0;
}
#else
MODULE_LICENSE("GPL");
int
init_module(void) {
  publisher(NULL);
  return 0;
}
void
cleanup_module(void) {
  ORTEDomainAppDestroy(d);
}
#endif
