/*
 *  $Id: RTPSAck.c,v 0.0.0.1            2003/10/07 
 *
 *  DEBUG:  section 47                  RTPS message ACK
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
int32_t 
RTPSAckCreate(u_int8_t *rtps_msg,u_int32_t max_msg_len,
    SequenceNumber *seqNumber,
    ObjectId roid,ObjectId woid,Boolean f_bit) {
  SequenceNumber        sn_tmp;                    
                    
  if (max_msg_len<28) return -1;
  rtps_msg[0]=(u_int8_t)ACK;
  rtps_msg[1]=ORTE_MY_MBO;
  if (f_bit) rtps_msg[1]|=2;
  *((ParameterLength*)(rtps_msg+2))=24;
  conv_u32(&roid,0);
  *((ObjectId*)(rtps_msg+4))=roid;
  conv_u32(&woid,0);
  *((ObjectId*)(rtps_msg+8))=woid;
  SeqNumberInc(sn_tmp,*seqNumber);
  *((SequenceNumber*)(rtps_msg+12))=sn_tmp;
  *((u_int32_t*)(rtps_msg+20))=32;
  *((u_int32_t*)(rtps_msg+24))=0;
  return 28;
} 

/**********************************************************************************/
void 
RTPSAck(ORTEDomain *d,u_int8_t *rtps_msg,MessageInterpret *mi,IPAddress senderIPAddress) {
  GUID_RTPS          readerGUID;
  CSTWriter          *cstWriter=NULL;
  CSTRemoteReader    *cstRemoteReader;
  CSChangeForReader  *csChangeForReader;
  StateMachineSend   stateMachineSendNew;
  ObjectId	         roid,woid;
  SequenceNumber     sn;   
  int8_t             e_bit,f_bit;

  e_bit=rtps_msg[1] & 0x01;
  f_bit=(rtps_msg[1] & 0x02)>>1;
  roid=*((ObjectId*)(rtps_msg+4));              /* readerObjectId */
  conv_u32(&roid,0);
  woid=*((ObjectId*)(rtps_msg+8));              /* writerObjectId */
  conv_u32(&woid,0);
  sn=*((SequenceNumber*)(rtps_msg+12));         /* Bitmap - SN    */
  conv_sn(&sn,e_bit);
  readerGUID.hid=mi->sourceHostId;
  readerGUID.aid=mi->sourceAppId;
  readerGUID.oid=roid;

  debug(47,3) ("recv: RTPS ACK%c(0x%x) from 0x%x-0x%x\n",
                f_bit ? 'F':'f',
                woid,mi->sourceHostId,mi->sourceAppId);
  
  //Manager
  if ((d->guid.aid & 0x03)==MANAGER) {
    switch (woid) {
      case OID_WRITE_APPSELF:
        pthread_rwlock_wrlock(&d->writerApplicationSelf.lock);
        cstWriter=&d->writerApplicationSelf;
        readerGUID.hid=senderIPAddress;
        readerGUID.aid=AID_UNKNOWN;
        readerGUID.oid=roid;
        break;
      case OID_WRITE_MGR:
        pthread_rwlock_wrlock(&d->writerManagers.lock);
        cstWriter=&d->writerManagers;
        break;
      case OID_WRITE_APP:
        pthread_rwlock_wrlock(&d->writerApplications.lock);
        cstWriter=&d->writerApplications;
        break;
    }
  }
  //Application
  if ((d->guid.aid & 0x03)==MANAGEDAPPLICATION) {
    switch (roid) {
      case OID_READ_APP:
      case OID_READ_APPSELF:
        pthread_rwlock_wrlock(&d->writerApplicationSelf.lock);
        cstWriter=&d->writerApplicationSelf;
        break;
      case OID_READ_PUBL:
        pthread_rwlock_wrlock(&d->writerPublications.lock);
        cstWriter=&d->writerPublications;
        break;
      case OID_READ_SUBS:
        pthread_rwlock_wrlock(&d->writerSubscriptions.lock);
        cstWriter=&d->writerSubscriptions;
        break;
    }
  }
  if (!cstWriter) return;
  cstRemoteReader=CSTRemoteReader_find(cstWriter,&readerGUID);
  if (!cstRemoteReader) {
    pthread_rwlock_unlock(&cstWriter->lock);
    return;
  }
  stateMachineSendNew=NOTHNIGTOSEND;
  gavl_cust_for_each(CSChangeForReader,cstRemoteReader,csChangeForReader) {
    if (SeqNumberCmp(csChangeForReader->csChange->sn,sn)<0)   {   //ACK
      if (csChangeForReader->commStateChFReader!=ACKNOWLEDGED) {
        csChangeForReader->commStateChFReader=ACKNOWLEDGED;
//        csChangeForReader->csChange->acknowledgedCounter++;
      }    
    } else {                                                      //NACK
      csChangeForReader->commStateChFReader=TOSEND;
      stateMachineSendNew=MUSTSENDDATA;
    }
  }
  if ((cstRemoteReader->commStateSend==NOTHNIGTOSEND) && 
      (stateMachineSendNew==MUSTSENDDATA)) {
    cstRemoteReader->commStateSend=stateMachineSendNew;
    eventDetach(d,
        cstRemoteReader->objectEntryOID->objectEntryAID,
        &cstRemoteReader->delayResponceTimer,
        1);
    eventAdd(d,
        cstRemoteReader->objectEntryOID->objectEntryAID,
        &cstRemoteReader->delayResponceTimer,
        1,   //metatraffic timer
        "CSTWriterSendTimer",
        CSTWriterSendTimer,
        &cstRemoteReader->cstWriter->lock,
        cstRemoteReader,
        &cstRemoteReader->cstWriter->params.delayResponceTime);               
  } 
  if ((cstRemoteReader->commStateSend==MUSTSENDDATA) && 
      (stateMachineSendNew==NOTHNIGTOSEND)) {
    cstRemoteReader->commStateSend=stateMachineSendNew;
    eventDetach(d,
        cstRemoteReader->objectEntryOID->objectEntryAID,
        &cstRemoteReader->delayResponceTimer,
        1);
  }  
  if ((!f_bit) && (cstRemoteReader->commStateSend==NOTHNIGTOSEND)) {
    eventDetach(d,
        cstRemoteReader->objectEntryOID->objectEntryAID,
        &cstRemoteReader->delayResponceTimer,
        1);
    eventAdd(d,
        cstRemoteReader->objectEntryOID->objectEntryAID,
        &cstRemoteReader->delayResponceTimer,
        1,   //metatraffic timer
        "CSTWriterSendTimer",
        CSTWriterSendTimer,
        &cstRemoteReader->cstWriter->lock,
        cstRemoteReader,
        &cstRemoteReader->cstWriter->params.delayResponceTime);               
  } 
  pthread_rwlock_unlock(&cstWriter->lock);
} 


