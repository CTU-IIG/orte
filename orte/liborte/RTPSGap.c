/*
 *  $Id: RTPSGap.c,v 0.0.0.1            2003/10/07 
 *
 *  DEBUG:  section 49                  RTPS message GAP
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
RTPSGapAdd(CSTRemoteWriter *cstRemoteWriter,GUID_RTPS *guid,SequenceNumber *fsn,
    SequenceNumber *sn,u_int32_t numbits,u_int8_t *bitmaps,Boolean e_bit) {
  SequenceNumber      lsn,ssn;
  u_int32_t           i;
  int8_t              bit,bit_last=0;
  CSChange            *csChange;
  u_int32_t           bitmap;
  
  if (SeqNumberCmp(*sn,cstRemoteWriter->sn)<0) return;//have to be sn>=writer_sn ! 
  if (SeqNumberCmp(*fsn,*sn)==1) return;              //cannot be fsn>sn ! 
  if (numbits>256) return;
  //first case of GAP sn
  if (SeqNumberCmp(*fsn,*sn)<0) {                        //if fsn<sn  
    if (!CSChangeFromWriter_find(cstRemoteWriter,fsn)) {
      if (SeqNumberCmp(*fsn,cstRemoteWriter->sn)>0) {    //have to be sn>writer_sn
        csChange=(CSChange*)MALLOC(sizeof(CSChange));
        csChange->cdrStream.buffer=NULL;
        csChange->sn=*fsn;
        csChange->guid=*guid;
        csChange->alive=ORTE_TRUE;
        SeqNumberSub(csChange->gapSN,*sn,*fsn);         //setup flag GAP
        CSChangeAttributes_init_head(csChange);
        CSTReaderAddCSChange(cstRemoteWriter,csChange);
      }
    }
  }
  //second case of GAP sn
  lsn=ssn=*sn;bit=0;
  for(i=0;i<numbits;i++) {
    bitmap=*(((u_int32_t*)bitmaps)+i/32);
    conv_u32(&bitmap,e_bit);
    bit=(bitmap & (1<<(31-i%32))) ? 1:0;
    if (i>0) {
      if (bit_last && !bit) {                           //end of GAP     1->0
        if (!CSChangeFromWriter_find(cstRemoteWriter,&ssn)) {
          if (SeqNumberCmp(ssn,cstRemoteWriter->sn)>0) {  
            csChange=(CSChange*)MALLOC(sizeof(CSChange));
            csChange->cdrStream.buffer=NULL;
            csChange->sn=ssn;
            csChange->guid=*guid;
            csChange->alive=ORTE_TRUE;
            SeqNumberSub(csChange->gapSN,lsn,ssn);         //setup flag GAP
            CSChangeAttributes_init_head(csChange);
            CSTReaderAddCSChange(cstRemoteWriter,csChange);
          }
        }
      } else {
        if (!bit_last && bit) {                         //begin GAP      0->1
          ssn=lsn;                                      //start pointer
        }
      }
    }  
    SeqNumberInc(lsn,lsn);
    bit_last=bit;
  }
  if (bit) {
    if (!CSChangeFromWriter_find(cstRemoteWriter,&ssn)) {
      if (SeqNumberCmp(ssn,cstRemoteWriter->sn)>0) {  
        csChange=(CSChange*)MALLOC(sizeof(CSChange));
        csChange->cdrStream.buffer=NULL;
        csChange->sn=ssn;
        csChange->guid=*guid;
        csChange->alive=ORTE_TRUE;
        SeqNumberSub(csChange->gapSN,lsn,ssn);         //setup flag GAP
        CSChangeAttributes_init_head(csChange);
        CSTReaderAddCSChange(cstRemoteWriter,csChange);
      }
    }
  }
}

/**********************************************************************************/
void 
RTPSGap(ORTEDomain *d,u_int8_t *rtps_msg,MessageInterpret *mi,IPAddress senderIPAddress) {
  CSTReader          *cstReader=NULL;
  CSTRemoteWriter    *cstRemoteWriter;
  GUID_RTPS          writerGUID;
  ObjectId	         roid,woid;
  SequenceNumber     sn,fsn;
  u_int32_t          numbits;
  int8_t             e_bit;

  e_bit=rtps_msg[1] & 0x01;
  roid=*((ObjectId*)(rtps_msg+4));              /* readerObjectId */
  conv_u32(&roid,0);
  woid=*((ObjectId*)(rtps_msg+8));              /* writerObjectId */
  conv_u32(&woid,0);
  fsn=*((SequenceNumber*)(rtps_msg+12));        /* firstSeqNumber */
  conv_sn(&fsn,e_bit);
  sn=*((SequenceNumber*)(rtps_msg+20));         /* Bitmap - SN    */
  conv_sn(&sn,e_bit);
  numbits=*((u_int32_t*)(rtps_msg+28));         /* numbits */
  conv_u32(&numbits,e_bit);
  writerGUID.hid=mi->sourceHostId;
  writerGUID.aid=mi->sourceAppId;
  writerGUID.oid=woid;

  debug(49,3) ("recv: RTPS_GAP(0x%x) from 0x%x-0x%x\n",
                woid,mi->sourceHostId,mi->sourceAppId);
  
  //Manager
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
  RTPSGapAdd(cstRemoteWriter,&writerGUID,&fsn,&sn,numbits,
             rtps_msg+32,e_bit);
  CSTReaderProcCSChanges(d,cstRemoteWriter);
  pthread_rwlock_unlock(&cstReader->lock);
} 


