/*
 *  $Id: subscriberreliable.c,v 0.0.0.1 2003/12/27 
 *
 *  DEBUG:  section                     subscriber reliable
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
#endif
#include "orte.h"

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
      printf("deadline occurred\n");
      break;
  }
}


int 
main(int argc, char *args[]) {
  ORTESubscription *s;
  NtpTime         deadline,minimumSeparation;

  ORTEInit();
  //ORTEVerbositySetOptions("ALL,10");
  d=ORTEDomainAppCreate(ORTE_DEFAULT_DOMAIN,NULL,NULL,ORTE_FALSE);
  ORTETypeRegisterAdd(d,"HelloMsg",NULL,NULL,NULL,sizeof(instance2Recv));
  NTPTIME_BUILD(deadline,3); 
  NTPTIME_BUILD(minimumSeparation,0); 
  s=ORTESubscriptionCreate(
       d,
       IMMEDIATE,
       STRICT_RELIABLE,
       "Reliable HelloMsg",
       "HelloMsg",
       &instance2Recv,
       &deadline,
       &minimumSeparation,
       recvCallBack,
       NULL,
       IPADDRESS_INVALID);
  #ifndef CONFIG_ORTE_RT
  while (1) 
    ORTESleepMs(1000);
  #endif
  return 0;
}

#ifdef CONFIG_ORTE_RT
void 
hello_init(void) {
  main(0,NULL);
}
void 
hello_exit(void) {
  ORTEDomainAppDestroy(d);
}
MODULE_LICENSE("GPL");
module_init(hello_init);
module_exit(hello_exit);
#endif

