/*
 *  $Id: ORTEAppSendThread.c,v 0.0.0.1  2003/08/21 
 *
 *  DEBUG:  section 24                  Sending thread
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
void ORTESendData(ORTEDomain *d,ObjectEntryAID *objectEntryAID,Boolean meta) {
  struct sockaddr_in  des; 
  ObjectEntryOID      *objectEntryOID;
  AppParams           *appParams;
  ObjectId            oid=OID_APP;
  int                 i;

  objectEntryOID=ObjectEntryOID_find(objectEntryAID,&oid);
  if (objectEntryOID) {
    appParams=(AppParams*)objectEntryOID->attributes;
    for(i=0;i<appParams->unicastIPAddressCount;i++) {
      des.sin_family=AF_INET; 
      des.sin_addr.s_addr = htonl(appParams->unicastIPAddressList[i]);
      if (meta) {
        des.sin_port = htons((uint16_t)appParams->metatrafficUnicastPort); 
        sock_sendto (
            &d->taskSend.sock,
            d->mbSend.cdrStream.buffer,
            d->mbSend.cdrStream.length,
            &des,
            sizeof(des)); 
      } else {
        des.sin_port = htons((uint16_t)appParams->userdataUnicastPort); 
        if (d->mbSend.cdrStreamDirect)
          sock_sendto (
              &d->taskSend.sock,
              d->mbSend.cdrStreamDirect->buffer,
              d->mbSend.cdrStreamDirect->length,
              &des,
              sizeof(des)); 
        else
          sock_sendto (
              &d->taskSend.sock,
              d->mbSend.cdrStream.buffer,
              d->mbSend.cdrStream.length,
              &des,
              sizeof(des)); 
      }
    }
  }
  //prepare buffer for next sending
  d->mbSend.cdrStream.length=RTPS_HEADER_LENGTH;
  d->mbSend.cdrStream.bufferPtr=d->mbSend.cdrStream.buffer+RTPS_HEADER_LENGTH;
  d->mbSend.needSend=ORTE_FALSE;
  d->mbSend.containsInfoReply=ORTE_FALSE;
}

/*****************************************************************************/
void ORTEAppSendThread(ORTEDomain *d) {
  struct timespec     wtime; 
  NtpTime             actTime,nextExpire,whenExpire,sleepingTime;
  int32_t             s,ms;

  debug(24,10) ("ORTEAppSendThread: start\n");
  d->objectEntry.htimNeedWakeUp=ORTE_TRUE;

  while (!d->taskSend.terminate) {
    actTime=getActualNtpTime();
    pthread_rwlock_wrlock(&d->objectEntry.htimRootLock);
    if (htimerRoot_next_expire(&d->objectEntry,&nextExpire)==0) {
      NTPTIME_BUILD(whenExpire,300); //max time for sleeping (no. events)
      NtpTimeAdd(nextExpire,actTime,whenExpire);
    }
    pthread_rwlock_unlock(&d->objectEntry.htimRootLock);
    NtpTimeDisAssembToUs(wtime.tv_sec,wtime.tv_nsec,nextExpire);
    wtime.tv_nsec*=1000;  //conver to nano seconds
    NtpTimeSub(sleepingTime,nextExpire,actTime);
    NtpTimeDisAssembToMs(s,ms,sleepingTime);
    if (s<0) s=ms=0;
    debug(24,4) ("ORTEAppSendThread: sleeping for %lis %lims\n",s,ms);
    if (!((wtime.tv_sec==0) && (wtime.tv_nsec==0))) {
      pthread_mutex_lock(&d->objectEntry.htimSendMutex);
      if (d->objectEntry.htimSendCondValue==0) {
        pthread_cond_timedwait(&d->objectEntry.htimSendCond,
	                       &d->objectEntry.htimSendMutex,
			       &wtime);
      }
      d->objectEntry.htimSendCondValue=0;
      pthread_mutex_unlock(&d->objectEntry.htimSendMutex);
    }
    debug(24,7) ("ORTEAppSendThread: fired\n");
    actTime=getActualNtpTime();
    pthread_rwlock_wrlock(&d->objectEntry.objRootLock);
    pthread_rwlock_wrlock(&d->objectEntry.htimRootLock);
    d->objectEntry.htimNeedWakeUp=ORTE_FALSE;
    htimerRoot_run_expired(d,&actTime);
    d->objectEntry.htimNeedWakeUp=ORTE_TRUE;
    pthread_rwlock_unlock(&d->objectEntry.htimRootLock);
    pthread_rwlock_unlock(&d->objectEntry.objRootLock);
  }
  debug(24,10) ("ORTEAppSendThread: finished\n");
  pthread_exit(NULL);
}

