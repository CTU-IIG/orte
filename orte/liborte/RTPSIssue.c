/*
 *  $Id: RTPSIssue.c,v 0.0.0.1          2003/12/08
 *
 *  DEBUG:  section 56                  message ISSUE
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

#include "orte_all.h"

/**********************************************************************************/
int32_t
RTPSIssueCreateHeader(uint8_t *rtps_msg,uint32_t max_msg_len,uint32_t length,
    ObjectId roid,ObjectId woid,SequenceNumber sn) {
  
  if (max_msg_len<20) return -1;
  rtps_msg[0]=(uint8_t)ISSUE;
  rtps_msg[1]=ORTE_MY_MBO;
  *((ParameterLength*)(rtps_msg+2))=(uint16_t)length;
  conv_u32(&roid,0);
  *((ObjectId*)(rtps_msg+4))=roid;
  conv_u32(&woid,0);
  *((ObjectId*)(rtps_msg+8))=woid;
  *((SequenceNumber*)(rtps_msg+12))=sn;
  return 0;
}

/**********************************************************************************/
void 
RTPSIssue(ORTEDomain *d,uint8_t *rtps_msg,MessageInterpret *mi,IPAddress senderIPAddress) {
  GUID_RTPS          guid,writerGUID;
  ObjectId           roid,woid;
  SequenceNumber     sn,sn_tmp;   
  int8_t             e_bit,p_bit;
  uint16_t           submsg_len;
  CSTReader          *cstReader;
  CSTRemoteWriter    *cstRemoteWriter;
  CSChange           *csChange=NULL;

  e_bit=rtps_msg[1] & 0x01;
  p_bit=(rtps_msg[1] & 0x02)>>1;
  submsg_len=*((uint16_t*)(rtps_msg+2));
  conv_u16(&submsg_len,e_bit);
  roid=*((ObjectId*)(rtps_msg+4));              /* readerObjectId */
  conv_u32(&roid,0);
  woid=*((ObjectId*)(rtps_msg+8));              /* writerObjectId */
  conv_u32(&woid,0);
  sn=*((SequenceNumber*)(rtps_msg+12));         /* sn             */
  conv_sn(&sn,e_bit);
  if (p_bit) return;       /* at this moment is not supported p_bit */
  writerGUID.hid=mi->sourceHostId;
  writerGUID.aid=mi->sourceAppId;
  writerGUID.oid=woid;
  
  debug(56,3) ("recv: RTPS_ISSUE(0x%x) from 0x%x-0x%x\n",
                woid,mi->sourceHostId,mi->sourceAppId);

  pthread_rwlock_rdlock(&d->subscriptions.lock);
  guid=d->guid;
  guid.oid=roid;
  gavl_cust_for_each(CSTReader,&d->subscriptions,cstReader) {
    if (roid!=OID_UNKNOWN)
      cstReader=CSTReader_find(&d->subscriptions,&guid);
    if (cstReader) {
      ORTESubsProp *sp;
      pthread_rwlock_wrlock(&cstReader->lock);
      sp=(ORTESubsProp*)cstReader->objectEntryOID->attributes;
      cstRemoteWriter=CSTRemoteWriter_find(cstReader,&writerGUID);
      if (cstRemoteWriter) {
        ORTEPublProp *pp,*pps;
        pp=(ORTEPublProp*)cstRemoteWriter->objectEntryOID->attributes;
        if (cstReader->cstRemoteWriterSubscribed!=NULL) {
          pps=(ORTEPublProp*)cstReader->cstRemoteWriterSubscribed->
                            objectEntryOID->attributes;
          if (pp->strength>pps->strength) {
            cstReader->cstRemoteWriterSubscribed=cstRemoteWriter;
          }
        } else {
          cstReader->cstRemoteWriterSubscribed=cstRemoteWriter;
        }
        if (cstReader->cstRemoteWriterSubscribed==cstRemoteWriter) {
          eventDetach(d,
              cstReader->objectEntryOID->objectEntryAID,
              &cstReader->persistenceTimer,
              0);   //common timer
          eventAdd(d,
              cstReader->objectEntryOID->objectEntryAID,
              &cstReader->persistenceTimer,
              0,   //common timer
              "CSTReaderPersistenceTimer",
              CSTReaderPersistenceTimer,
              &cstReader->lock,
              cstReader,
              &pp->persistence);
        }
        if ((SeqNumberCmp(sn,cstRemoteWriter->sn)>0) &&   //have to be sn>writer_sn
            (CSChangeFromWriter_find(cstRemoteWriter,&sn)==NULL)) {
          csChange=(CSChange*)MALLOC(sizeof(CSChange));
          csChange->cdrStream.buffer=NULL;
          csChange->guid=writerGUID;
          csChange->sn=sn;
          SEQUENCE_NUMBER_NONE(csChange->gapSN);
          CSChangeAttributes_init_head(csChange);
          csChange->cdrStream.length=submsg_len-16;
          csChange->cdrStream.buffer=(int8_t*)MALLOC(submsg_len-16);
          csChange->cdrStream.bufferPtr=csChange->cdrStream.buffer;
          csChange->cdrStream.needByteSwap=ORTE_FALSE;
          if (e_bit ^ ORTE_MY_MBO)        
            csChange->cdrStream.needByteSwap=ORTE_TRUE;
          memcpy(csChange->cdrStream.buffer,rtps_msg+20,submsg_len-16);
          if (SeqNumberCmp(sn,cstRemoteWriter->firstSN)>=0) { //sn>=firstSN
            if ((sp->reliabilityRequested & PID_VALUE_RELIABILITY_STRICT)!=0) {
              if (sp->recvQueueSize>cstRemoteWriter->csChangesCounter) {
                sn_tmp.high=0;
                sn_tmp.low=sp->recvQueueSize;
                SeqNumberAdd(sn_tmp,
                             cstRemoteWriter->sn,
                             sn_tmp);
                if (SeqNumberCmp(sn,sn_tmp)<=0) {         //sn<=(firstSN+QueueSize)
                  csChange->remoteTimePublished=mi->timestamp;
                  csChange->localTimeReceived=getActualNtpTime();
                  CSTReaderAddCSChange(cstRemoteWriter,csChange);
                  csChange=NULL;
                }
              }
            } else {
              if ((sp->reliabilityRequested & PID_VALUE_RELIABILITY_BEST_EFFORTS)!=0) {
                if ((sp->recvQueueSize<=cstRemoteWriter->csChangesCounter) ||
                    (cstReader->cstRemoteWriterSubscribed!=cstRemoteWriter)) {
                  CSChangeFromWriter *csChangeFromWriter;
                  csChangeFromWriter=CSChangeFromWriter_first(cstRemoteWriter);
                  CSTReaderDestroyCSChangeFromWriter(cstRemoteWriter,
                        csChangeFromWriter,
                        ORTE_FALSE);
                }
                if (sp->recvQueueSize>cstRemoteWriter->csChangesCounter) {
                  csChange->remoteTimePublished=mi->timestamp;
                  csChange->localTimeReceived=getActualNtpTime();
                  CSTReaderAddCSChange(cstRemoteWriter,csChange);
                  csChange=NULL;
                }
              }
            }
          } 
        }
        if (csChange) {
          FREE(csChange->cdrStream.buffer);
          FREE(csChange);
        }
        CSTReaderProcCSChangesIssue(cstRemoteWriter,ORTE_FALSE);
      }
      pthread_rwlock_unlock(&cstReader->lock);  
    } else
      break;  //break traceing all cstReaders
  }
  pthread_rwlock_unlock(&d->subscriptions.lock);
}
