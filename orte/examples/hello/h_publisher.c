/*
 *  $Id: h_publisher.c,v 0.0.1.0        2005/01/03
 *
 *  DEBUG:  section                     h_publisher
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
#include "orte.h"
#ifdef CONFIG_ORTE_RTL
  #include <linux/module.h>
  #include <posix/pthread.h>
  #define printf rtl_printf
#elif defined CONFIG_ORTE_RTAI
  #include <linux/module.h>
  #include <rtai/compat.h>  
  #define printf rt_printk
#else
  #include <stdio.h>
#endif

ORTEDomain        *d=NULL;
char              instance2Send[64];
int               counter=0;

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
publisherCreate(void *arg) {
  ORTEPublication     *p;
  NtpTime             persistence, delay;

  ORTETypeRegisterAdd(d,"HelloMsg",NULL,NULL,NULL,sizeof(instance2Send));
  NTPTIME_BUILD(persistence,3);
  NTPTIME_BUILD(delay,1); 
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
  return arg;
}


#ifndef CONFIG_ORTE_RT

int
main(int argc, char *args[]) {
  ORTEInit();
  ORTEVerbositySetOptions("ALL.10");
  d=ORTEDomainAppCreate(ORTE_DEFAULT_DOMAIN,NULL,NULL,ORTE_FALSE);
  if (!d) {
    printf("ORTEDomainAppCreate failed!\n");
    return 0;
  }
  publisherCreate((void*)d);
  while(1) 
    ORTESleepMs(1000);

  ORTESleepMs(10000);
  printf("finnished!\n");
  ORTEDomainAppDestroy(d);
  return 0;
}

#else
char *verbosity="";
MODULE_PARM(verbosity,"1s");
MODULE_PARM_DESC(verbosity,"set verbosity level SECTION, up to LEVEL:...");
char *manager="127.0.0.1";
MODULE_PARM(manager,"1s");
MODULE_PARM_DESC(manager,"IP address of local manager");
MODULE_LICENSE("GPL");
pthread_t thread;

void *
domainInit(void *arg) {
  ORTEDomainProp dp;

  ORTEVerbositySetOptions(verbosity);
  ORTEDomainPropDefaultGet(&dp);
  dp.appLocalManager=StringToIPAddress(manager);
  d=ORTEDomainAppCreate(ORTE_DEFAULT_DOMAIN,&dp,NULL,ORTE_TRUE);
  return arg;
}

void *
domainDestroy(void *arg) {
  if (!d) return NULL;
  ORTEDomainAppDestroy(d);
  return arg;
}

int
init_module(void) {

  ORTEInit();
  pthread_create(&thread,NULL,&domainInit,NULL);  //allocate resources in RT 
  pthread_join(thread,NULL);
  if (d) {
    ORTEDomainStart(d,ORTE_TRUE,ORTE_FALSE,ORTE_TRUE,ORTE_FALSE,ORTE_TRUE); //application start
    if (pthread_create(&thread,NULL,&publisherCreate,NULL)!=0)
      printf("pthread_create failed!\n");
  } else
    printf("ORTEDomainAppCreate failed!\n");
  return 0;
}


void
cleanup_module(void) {
  if (!d) return;
  pthread_join(thread,NULL);
  pthread_create(&thread,NULL,&domainDestroy,NULL);
  pthread_join(thread,NULL);
}

#endif


