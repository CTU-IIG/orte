/*
 *  $Id: ORTEDomain.c,v 0.0.0.1         2003/08/21
 *
 *  DEBUG:  section 30                  Domain functions
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
ORTEDomainStart(ORTEDomain *d,
    Boolean recvMetatrafficThread,Boolean recvUserDataThread,Boolean sendThread) {
  if(!d) return;
  if ((recvMetatrafficThread) && (d->taskRecvMetatraffic.terminate)) {
    d->taskRecvMetatraffic.terminate=ORTE_FALSE;
    pthread_create(&d->taskRecvMetatraffic.thread, NULL,
                  (void*)&ORTEAppRecvMetatrafficThread, (void *)d); 
  }
  if ((recvUserDataThread) && (d->taskRecvUserdata.terminate)) {
    d->taskRecvUserdata.terminate=ORTE_FALSE;
    pthread_create(&d->taskRecvUserdata.thread, NULL,
                  (void*)&ORTEAppRecvUserdataThread, (void *)d); 
  }
  if ((sendThread) && (d->taskSend.terminate)) {
    d->taskSend.terminate=ORTE_FALSE;
    pthread_create(&d->taskSend.thread, NULL,
                  (void*)&ORTEAppSendThread, (void *)d); 
  }
}

/*****************************************************************************/
Boolean
ORTEDomainPropDefaultGet(ORTEDomainProp *prop) {
  sock_t        sock;

  memset(prop, 0, sizeof(*prop));

  prop->multicast.enabled=ORTE_FALSE;
  prop->multicast.ttl=1;
  prop->multicast.loopBackEnabled=ORTE_TRUE;

  //IFProp
  sock_init_udp(&sock);
  sock_bind(&sock,0);
  sock_get_local_interfaces(&sock,prop->IFProp,&prop->IFCount);
  sock_cleanup(&sock); 

  prop->mgrs=NULL; //only from localhost
  prop->appLocalManager=StringToIPAddress("127.0.0.1");
  prop->keys=NULL; //are assign be orte
  sprintf(prop->version,ORTE_PACKAGE_STRING\
                        ", compiled: "\
                        __DATE__\
                        " "\
                        __TIME__);
                        
  prop->recvBuffSize=0x4000;
  prop->sendBuffSize=0x4000; 
  prop->wireProp.metaBytesPerPacket=1500;
  prop->wireProp.metaBytesPerFastPacket=1000;
  prop->wireProp.metabitsPerACKBitmap=32;
  prop->wireProp.userMaxSerDeserSize=0x4000;
  
  //domainBaseProp
  NTPTIME_BUILD(prop->baseProp.expirationTime,180); //180s
  NTPTIME_BUILD(prop->baseProp.refreshPeriod,72);    //72s - refresh self parameters
  NTPTIME_BUILD(prop->baseProp.purgeTime,60);        //60s - purge time of parameters
  NTPTIME_BUILD(prop->baseProp.repeatAnnounceTime,72);//72s - announcement by HB
  NTPTIME_BUILD(prop->baseProp.repeatActiveQueryTime,72);//72s - announcement by ACK
  NtpTimeAssembFromMs(prop->baseProp.delayResponceTimeACKMin,0,10);//10ms - delay before send ACK
  NtpTimeAssembFromMs(prop->baseProp.delayResponceTimeACKMax,1,0);//1s
  NtpTimeAssembFromMs(prop->baseProp.maxBlockTime,20,0);//20s
  prop->baseProp.ACKMaxRetries=10;
  prop->baseProp.HBMaxRetries=10;
  
  PublParamsInit(&prop->publPropDefault);
  SubsParamsInit(&prop->subsPropDefault);
  
  return ORTE_TRUE;
}

/*****************************************************************************/
Boolean
ORTEDomainInitEvents(ORTEDomainAppEvents *events) {
  memset(events,0,sizeof(ORTEDomainAppEvents));
  return ORTE_TRUE;
}
