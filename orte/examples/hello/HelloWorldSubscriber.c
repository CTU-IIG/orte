/*
 *  $Id: HelloWorldSubscriber.c,v 0.0.0.1 2003/12/26 
 *
 *  DEBUG:  section                     HelloWorldSubscriber
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

#include <stdio.h>
#ifdef __RTL__
  #include <posix/pthread.h>
  #define printf rtl_printf
#endif
#include "orte_api.h"

ORTEDomain        *d;
char              instance2Recv[64];

void
recvCallBack(const ORTERecvInfo *info,void *vinstance, void *recvCallBackParam) {
  char *instance=(char*)vinstance;
  
  switch (info->status) {
    case NEW_DATA:
      printf("%s\n",instance);
      break;
    case DEADLINE:
      printf("deadline occured\n");
      break;
  }
}


void
subscriberCreate(void) {
  ORTESubscription    *s;
  ORTEDomainProp      dp; 
  NtpTime             deadline,minimumSeparation;

  ORTEInit();
  ORTEVerbositySetOptions("ALL,10");
  ORTEDomainPropDefaultGet(&dp);
  #ifdef __RTL__
  dp.appLocalManager=StringToIPAddress("192.168.4.1");
  #endif
  d=ORTEDomainAppCreate(ORTE_DEFAULT_DOMAIN,&dp,NULL);
  ORTETypeRegisterAdd(d,"HelloMsg",NULL,NULL,64);
  NTPTIME_BUILD(deadline,15); 
  NTPTIME_BUILD(minimumSeparation,0); 
  s=ORTESubscriptionCreate(
       d,
       IMMEDIATE,
       BEST_EFFORTS,
       "Example HelloMsg",
       "HelloMsg",
       &instance2Recv,
       &deadline,
       &minimumSeparation,
       recvCallBack,
       NULL);
}

#ifndef __RTL__
int 
main(int argc, char *args[]) {
  subscriberCreate();
  while (1) 
    ORTESleepMs(1000);
  return 0;
}
#else
pthread_t t;
int 
init_module(void) {
  subscriberCreate();
  return 0;
}
void 
cleanup_module(void) {
  ORTEDomainAppDestroy(d);
}
#endif

