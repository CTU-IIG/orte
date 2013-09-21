/*
 *  $Id: m_subscriber.c,v 0.0.1.0       2005/01/03
 *
 *  DEBUG:  section                     m_subscriber
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
 *  Author: 		 Petr Smolik	petr.smolik@wo.cz             
 *  Advisor: 		 Pavel Pisa                                   
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
#include "orte.h"
#include <stdio.h>

#ifdef MAIN_RENAMED
#define main orte_schneider_subscriber_main
#define exit return
#endif

static ORTEDomain        *d = NULL;
static char              instance2Recv[64];

int maxDataSize(ORTEGetMaxSizeParam *gms, int num) {
  return gms->max_size;
}

static void
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


static void *
subscriberCreate(void *arg) {
  ORTESubscription    *s;
  NtpTime             deadline,minimumSeparation;

  ORTETypeRegisterAdd(d,"IDA_OCTETSTRING",NULL,NULL,maxDataSize,sizeof(instance2Recv));
  NTPTIME_BUILD(deadline,10);
  NTPTIME_BUILD(minimumSeparation,0);
  s=ORTESubscriptionCreate(
       d,
       IMMEDIATE,
       BEST_EFFORTS,
       "IDA_TOPIC0001",
       "IDA_OCTETSTRING",
       &instance2Recv,
       &deadline,
       &minimumSeparation,
       recvCallBack,
       NULL,
//       StringToIPAddress("225.0.0.2")
       IPADDRESS_INVALID
       );
  if (s == NULL) {
    printf("ORTESubscriptionCreate failed\n");
  }
  return arg;
}

int
main(int argc, char *args[]) {
  ORTEDomainProp dp;
  
  ORTEInit();
  ORTEDomainPropDefaultGet(&dp);
//  dp.multicast.enabled=ORTE_TRUE;
//  dp.multicast.ipAddress=StringToIPAddress("225.0.0.1");
  ORTEVerbositySetOptions("ALL.5");
  d=ORTEDomainAppCreate(1,&dp,NULL,ORTE_FALSE);
  if (!d) {
    printf("ORTEDomainAppCreate failed!\n");
    return 0;
  }
  subscriberCreate(NULL);
  while (1)
    ORTESleepMs(1000);
  return 0;
}

