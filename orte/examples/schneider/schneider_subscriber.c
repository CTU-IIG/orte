/*
 *  $Id: m_subscriber.c,v 0.0.1.0       2005/01/03
 *
 *  DEBUG:  section                     m_subscriber
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
#include <stdio.h>

ORTEDomain        *d = NULL;
char              instance2Recv[64];

int maxDataSize(ORTEGetMaxSizeParam *gms) {
  return gms->max_size;
}

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


void *
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

