/*
 *  $Id: RTPSHeardBeat.c,v 0.0.0.1      2003/10/07 
 *
 *  DEBUG:  section 48                  RTPS message HeardBeat
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

/**********************************************************************************/
int 
RTPSHeardBeatCreate(u_int8_t *rtps_msg,u_int32_t max_msg_len,
    SequenceNumber *firstSeqNumber,SequenceNumber *lastSeqNumber,
    ObjectId woid,ObjectId roid,Boolean f_bit) {
  if (max_msg_len<28) return -1;
  rtps_msg[0]=(u_int8_t)HEARTBEAT;
  rtps_msg[1]=ORTE_MY_MBO;
  if (f_bit) rtps_msg[1]|=2;
  *((ParameterLength*)(rtps_msg+2))=24;
  conv_u32(&roid,0);
  *((ObjectId*)(rtps_msg+4))=roid;
  conv_u32(&woid,0);
  *((ObjectId*)(rtps_msg+8))=woid;
  *((SequenceNumber*)(rtps_msg+12))=*firstSeqNumber;
  *((SequenceNumber*)(rtps_msg+20))=*lastSeqNumber;
  return 28;
}

/**********************************************************************************/
void 
RTPSHeardBeat(ORTEDomain *d,u_int8_t *rtps_msg,MessageInterpret *mi) {
  GUID_RTPS          writerGUID;
  ObjectId	         roid,woid;
  SequenceNumber     fsn,lsn;
  int8_t             e_bit,f_bit;
  CSTReader          *cstReader=NULL;
  CSTRemoteWriter    *cstRemoteWriter=NULL;

  e_bit=rtps_msg[1] & 0x01;
  f_bit=(rtps_msg[1] & 0x02)>>1;
  roid=*((ObjectId*)(rtps_msg+4));              /* readerObjectId */
  conv_u32(&roid,0);
  woid=*((ObjectId*)(rtps_msg+8));              /* writerObjectId */
  conv_u32(&woid,0);
  fsn=*((SequenceNumber*)(rtps_msg+12));        /* firstSeqNumber */
  conv_sn(&fsn,e_bit);
  lsn=*((SequenceNumber*)(rtps_msg+20));        /* lastSeqNumber  */
  conv_sn(&lsn,e_bit);
  if (SeqNumberCmp(fsn,lsn)==1) return;	        // lsn<fsn -> break
  writerGUID.hid=mi->sourceHostId;
  writerGUID.aid=mi->sourceAppId;
  writerGUID.oid=woid;

  debug(48,3) ("recv: RTPS HB%c(0x%x) from 0x%x-0x%x\n",
                f_bit ? 'F':'f',woid,mi->sourceHostId,mi->sourceAppId);

  if ((d->guid.aid & 0x03)==MANAGER) {
    if ((writerGUID.oid==OID_WRITE_APPSELF) && 
        ((writerGUID.aid & 0x03)==MANAGER)) {
      pthread_rwlock_wrlock(&d->readerManagers.lock);
      cstReader=&d->readerManagers;
    }
    if (((writerGUID.oid==OID_WRITE_APPSELF) &&
         ((writerGUID.aid & 0x03)==MANAGEDAPPLICATION)) ||
        ((writerGUID.oid==OID_WRITE_APP) &&
         ((writerGUID.aid & 0x03)==MANAGER))) {
      pthread_rwlock_wrlock(&d->readerApplications.lock);
      cstReader=&d->readerApplications;
    }
  }
  if ((d->guid.aid & 3)==MANAGEDAPPLICATION) {
    switch (writerGUID.oid) {
      case OID_WRITE_MGR:
        pthread_rwlock_wrlock(&d->readerManagers.lock);
        cstReader=&d->readerManagers;
        break;
      case OID_WRITE_APP:
        pthread_rwlock_wrlock(&d->readerApplications.lock);
        cstReader=&d->readerApplications;
        break;
      case OID_WRITE_PUBL:
        pthread_rwlock_wrlock(&d->readerPublications.lock);
        cstReader=&d->readerPublications;
        break;
      case OID_WRITE_SUBS:
        pthread_rwlock_wrlock(&d->readerSubscriptions.lock);
        cstReader=&d->readerSubscriptions;
        break;
    }
  }  
  if (!cstReader) return;
  cstRemoteWriter=CSTRemoteWriter_find(cstReader,&writerGUID);
  if (!cstRemoteWriter) {
    pthread_rwlock_unlock(&cstReader->lock);
    return;
  }
  cstRemoteWriter->firstSN=fsn;
  cstRemoteWriter->lastSN=lsn;
  cstRemoteWriter->ACKRetriesCounter=0;
  if (SeqNumberCmp(cstRemoteWriter->sn,lsn)>0)
    cstRemoteWriter->sn=lsn;
  if (SeqNumberCmp(cstRemoteWriter->sn,fsn)<0) {
    if (SeqNumberCmp(fsn,noneSN)!=0) {
      SeqNumberDec(cstRemoteWriter->sn,fsn);
    }
  }
  CSTReaderProcCSChanges(d,cstRemoteWriter);
  if ((!f_bit) && (cstRemoteWriter->commStateACK==WAITING)) {
    cstRemoteWriter->commStateACK=ACKPENDING;
    eventDetach(d,
        cstRemoteWriter->objectEntryOID->objectEntryAID,
        &cstRemoteWriter->repeatActiveQueryTimer,
        1); 
    eventDetach(d,
        cstRemoteWriter->objectEntryOID->objectEntryAID,
        &cstRemoteWriter->delayResponceTimer,
        1);   //metatraffic timer
    eventAdd(d,
        cstRemoteWriter->objectEntryOID->objectEntryAID,
        &cstRemoteWriter->delayResponceTimer,
        1,   //metatraffic timer
        "CSTReaderResponceTimer",
        CSTReaderResponceTimer,
        &cstRemoteWriter->cstReader->lock,
        cstRemoteWriter,
        &cstRemoteWriter->cstReader->params.delayResponceTimeMin);
  }
  pthread_rwlock_unlock(&cstReader->lock);
} 
