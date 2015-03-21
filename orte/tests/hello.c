/*  -------------------------------------------------------------------
 *                                ORTE
 *                      Open Real-Time Ethernet
 *
 *  Copyright (C) 2015 Czech Technical University in Prague
 *  http://dce.felk.cvut.cz
 *
 *  Author:              Michal Sojka <sojkam1@fel.cvut.cz>
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
 *  --------------------------------------------------------------------
 *
 *  Description: Test that combines manager, publisher and subscriber
 *  in a single process.
 */


#include <orte.h>
#include <stdio.h>
#include <semaphore.h>

sem_t done;

static void
sendCallBack(const ORTESendInfo *info, void *vinstance, void *sendCallBackParam)
{
  char *instance = (char *)vinstance;
  static int counter;

  switch (info->status) {
    case NEED_DATA:
      sprintf(instance, "Hello Universe! (%d)", counter++);
      printf("Publishing: %s\n", instance);
      break;
    case CQL:  //criticalQueueLevel
      break;
  }
}

static void
publisherCreate(ORTEDomain *d)
{
  ORTEPublication     *p;
  NtpTime             persistence = NTPTIME_INITIALIZER(3);
  NtpTime             delay = NTPTIME_INITIALIZER(1);
  static char         instance2Send[64];

  ORTETypeRegisterAdd(d, "HelloMsg", NULL, NULL, NULL, sizeof(instance2Send));
  p = ORTEPublicationCreate(d,                  /* domain */
			    "Example HelloMsg", /* topic */
			    "HelloMsg",         /* type */
			    &instance2Send,     /* instance */
			    &persistence,       /* persistence */
			    1,                  /* strength */
			    sendCallBack,       /* callback */
			    NULL,               /* callback param */
			    &delay);            /* callback delay */
  if (p == NULL)
    printf("ORTEPublicationCreate failed\n");
}

static void
recvCallBack(const ORTERecvInfo *info, void *vinstance, void *recvCallBackParam)
{
  char *instance = (char *)vinstance;

  switch (info->status) {
    case NEW_DATA:
      printf("Received: %s\n", instance);
      sem_post(&done);
      break;
    case DEADLINE:
      printf("deadline occurred\n");
      break;
  }
}

static void
subscriberCreate(ORTEDomain *d)
{
  ORTESubscription    *s;
  NtpTime             deadline = NTPTIME_INITIALIZER(10);
  NtpTime             minimumSeparation = NTPTIME_INITIALIZER(0);
  static char         instance2Recv[64];

  ORTETypeRegisterAdd(d, "HelloMsg", NULL, NULL, NULL, sizeof(instance2Recv));
  s = ORTESubscriptionCreate(d,
			     IMMEDIATE,
			     BEST_EFFORTS,
			     "Example HelloMsg",
			     "HelloMsg",
			     &instance2Recv,
			     &deadline,
			     &minimumSeparation,
			     recvCallBack,
			     NULL,
			     IPADDRESS_INVALID);
  if (s == NULL)
    printf("ORTESubscriptionCreate failed\n");
}


int
main(int argc, char *argv[])
{
  ORTEDomain *dm, *dp, *ds;
  ORTEDomainProp dprop;

  sem_init(&done, 0, 0);

  ORTEInit();
  ORTEVerbositySetOptions("ALL.10");

  /* Manager */
  ORTEDomainPropDefaultGet(&dprop);
  dm = ORTEDomainMgrCreate(0, &dprop, NULL, ORTE_FALSE);

  /* Publisher */
  dp = ORTEDomainAppCreate(0, NULL, NULL, ORTE_FALSE);
  publisherCreate(dp);

  /* Subscriber */
  ds = ORTEDomainAppCreate(0, NULL, NULL, ORTE_FALSE);
  subscriberCreate(ds);

  sem_wait(&done);

  ORTEDomainAppDestroy(ds);
  ORTEDomainAppDestroy(dp);
  ORTEDomainMgrDestroy(dm);

  return 0;
}
