/*
 *  $Id: ORTEThreadUtils.c,v 0.0.0.1    2003/08/21 
 *
 *  DEBUG:  section 25                   Thread utility
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

/*****************************************************************************/
void
ORTEDomainWakeUpReceivingThread(ORTEDomain *d,sock_t *sock,u_int16_t port) {
  struct sockaddr_in    des;
  char                  sIPAddress[MAX_STRING_IPADDRESS_LENGTH];
  int                   i;

  des.sin_family = AF_INET;
  des.sin_port = htons(port);             //to receiving port
  if (d->domainProp.IFCount) {
    for(i=0;i<d->domainProp.IFCount;i++) {
      des.sin_addr.s_addr=htonl(d->domainProp.IFProp[i].ipAddress);
      sock_sendto(sock,&i,1,&des,sizeof(des));
      debug(25,2) ("Sent wake up signal to: %s.%d\n",
                    IPAddressToString(ntohl(des.sin_addr.s_addr),sIPAddress),
      port);
    }
  } else {
    des.sin_addr.s_addr = inet_addr("127.0.0.1"); //local IPAddress
    sock_sendto(sock,NULL,0,&des,sizeof(des));
    debug(25,2) ("Sent wake up signal to: %s.%d\n",
                  IPAddressToString(ntohl(des.sin_addr.s_addr),sIPAddress),
                  port);
  }
}

/*****************************************************************************/
void
ORTEDomainWakeUpSendingThread(ObjectEntry *objectEntry) {
  debug(25,10) ("WakeUpSendingThread : start\n");
  if (objectEntry->htimNeedWakeUp) {
    int value;
    sem_getvalue(&objectEntry->htimSendSem,&value);
    debug(25,8) ("WakeUpSendingThread : value:%d\n",value);
    if (value<1)
      sem_post(&objectEntry->htimSendSem);
  }
}



