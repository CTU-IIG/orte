/*
 *  $Id: RTPSCSTWriterTimer.c,v 0.0.0.1 2003/10/19 
 *
 *  DEBUG:  section 52                  CSTWriter timer functions
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
int 
CSTWriterRefreshTimer(ORTEDomain *d,void *vcstWriter) {
  CSTWriter *cstWriter=(CSTWriter*)vcstWriter;
  CSTRemoteReader *cstRemoteReader;
  
  debug(52,10) ("CSTWriterRefreshTimer: start\n");
  
  gavl_cust_for_each(CSTRemoteReader,cstWriter,cstRemoteReader) {
    CSTWriterRefreshAllCSChanges(d,cstRemoteReader);
  }
  eventDetach(d,
      cstWriter->objectEntryOID->objectEntryAID,
      &cstWriter->refreshPeriodTimer,
      0);   //common timer
  eventAdd(d,
      cstWriter->objectEntryOID->objectEntryAID,
      &cstWriter->refreshPeriodTimer,
      0,   //common timer
      "CSTWriterRefreshTimer",
      CSTWriterRefreshTimer,
      &cstWriter->lock,
      cstWriter,
      &cstWriter->params.refreshPeriod);               
  debug(52,10) ("CSTWriterRefreshTimer: finished\n");
  return 0;
}

/*****************************************************************************/
int 
CSTWriterAnnounceTimer(ORTEDomain *d,void *vcstRemoteReader) {
  CSTRemoteReader *cstRemoteReader=(CSTRemoteReader*)vcstRemoteReader;

  debug(52,10) ("CSTWriterAnnounceTimer: start\n");
  if ((cstRemoteReader->commStateHB==MAYSENDHB) &&
      ((!cstRemoteReader->cstWriter->params.fullAcknowledge))) {// ||
//       (cstRemoteReader->unacknowledgedCounter))) {
    //create HB
    int len=RTPSHeardBeatCreate(
        d->mbSend.cdrStream.bufferPtr,
        getMaxMessageLength(d),
        &cstRemoteReader->cstWriter->firstSN,
        &cstRemoteReader->cstWriter->lastSN,
        cstRemoteReader->cstWriter->guid.oid,
        OID_UNKNOWN,
        ORTE_FALSE);
    if (len<0) {
      //not enought space in sending buffer
      d->mbSend.needSend=ORTE_TRUE;
      return 1;
    }
    d->mbSend.cdrStream.bufferPtr+=len;
    d->mbSend.cdrStream.length+=len;
    debug(52,3) ("sent: RTPS_HBF(0x%x) to 0x%x-0x%x\n",
                  cstRemoteReader->cstWriter->guid.oid,
                  cstRemoteReader->guid.hid,
                  cstRemoteReader->guid.aid);
  }
  eventDetach(d,
      cstRemoteReader->objectEntryOID->objectEntryAID,
      &cstRemoteReader->repeatAnnounceTimer,
      1);
  eventAdd(d,
      cstRemoteReader->objectEntryOID->objectEntryAID,
      &cstRemoteReader->repeatAnnounceTimer,
      1,   //metatraffic timer
      "CSTWriterAnnounceTimer",
      CSTWriterAnnounceTimer,
      &cstRemoteReader->cstWriter->lock,
      cstRemoteReader,
      &cstRemoteReader->cstWriter->params.repeatAnnounceTime);
  debug(52,10) ("CSTWriterAnnounceTimer: finished\n");
  return 0;
}

/*****************************************************************************/
int 
CSTWriterAnnounceIssueTimer(ORTEDomain *d,void *vcstRemoteReader) {
  CSTRemoteReader *cstRemoteReader=(CSTRemoteReader*)vcstRemoteReader;
  NtpTime         nextHB;
  ORTEPublProp    *pp;
  int             len;

  debug(52,10) ("CSTWriterAnnounceIssueTimer: start\n");
  pp=(ORTEPublProp*)cstRemoteReader->cstWriter->objectEntryOID->attributes;
  //create HB
  d->mbSend.cdrStreamDirect=NULL;
  len=RTPSHeardBeatCreate(
      d->mbSend.cdrStream.bufferPtr,
      getMaxMessageLength(d),
      &cstRemoteReader->cstWriter->firstSN,
      &cstRemoteReader->cstWriter->lastSN,
      cstRemoteReader->cstWriter->guid.oid,
      OID_UNKNOWN,
      ORTE_FALSE);
  if (len<0) {
    //not enought space in sending buffer
    d->mbSend.needSend=ORTE_TRUE;
    return 1;
  }
  d->mbSend.cdrStream.bufferPtr+=len;
  d->mbSend.cdrStream.length+=len;
  debug(52,3) ("sent: RTPS_HBF(0x%x) to 0x%x-0x%x\n",
                cstRemoteReader->cstWriter->guid.oid,
                cstRemoteReader->guid.hid,
                cstRemoteReader->guid.aid);
  //next HB
  if (cstRemoteReader->cstWriter->csChangesCounter>=pp->criticalQueueLevel) {
    nextHB=pp->HBCQLRate;
  } else {
    nextHB=pp->HBNornalRate;
  }
  cstRemoteReader->HBRetriesCounter++;
  eventDetach(d,
      cstRemoteReader->objectEntryOID->objectEntryAID,
      &cstRemoteReader->repeatAnnounceTimer,
      2);
  if (cstRemoteReader->HBRetriesCounter<pp->HBMaxRetries) {              
    eventAdd(d,
        cstRemoteReader->objectEntryOID->objectEntryAID,
        &cstRemoteReader->repeatAnnounceTimer,
        2,   //metatraffic timer
        "CSTWriterAnnounceIssueTimer",
        CSTWriterAnnounceIssueTimer,
        &cstRemoteReader->cstWriter->lock,
        cstRemoteReader,
        &nextHB);
  } else {
    //destroy all csChangesForReader
    CSChangeForReader *csChangeForReader;
    while ((csChangeForReader=CSChangeForReader_first(cstRemoteReader))) {
      CSTWriterDestroyCSChangeForReader(cstRemoteReader,
          csChangeForReader,ORTE_TRUE);
    }
    debug(52,3) ("CSTWriterAnnounceIssueTimer: HB RR(0x%x-0x%x) ritch MaxRetries\n",
                  cstRemoteReader->guid.hid,cstRemoteReader->guid.aid);
  }
  debug(52,10) ("CSTWriterAnnounceIssueTimer: finished\n");
  return 0;
}

/**********************************************************************************/
int
CSChangeForReaderUnderwayTimer(ORTEDomain *d,void *vcsChangeForReader) {
  CSChangeForReader *csChangeForReader=(CSChangeForReader*)vcsChangeForReader;
  csChangeForReader->commStateChFReader=UNACKNOWLEDGED;
  return 0;
}

/**********************************************************************************/
int
CSTWriterSendBestEffortTimer(ORTEDomain *d,void *vcstRemoteReader) {
  CSTRemoteReader   *cstRemoteReader=(CSTRemoteReader*)vcstRemoteReader;
  ORTESubsProp      *sp=(ORTESubsProp*)cstRemoteReader->objectEntryOID->attributes;
  CSChangeForReader *csChangeForReader=NULL;
        
  debug(52,10) ("CSTWriterSendBestEffortTimer: start\n");
  d->mbSend.cdrStreamDirect=NULL;
  if (cstRemoteReader->commStateSend!=NOTHNIGTOSEND) {
    gavl_cust_for_each(CSChangeForReader,cstRemoteReader,csChangeForReader) {
      if (csChangeForReader->commStateChFReader==TOSEND) {
        CSChange *csChange=csChangeForReader->csChange;
        csChangeForReader->commStateChFReader=UNDERWAY;
        cstRemoteReader->commStateSend=MUSTSENDDATA;
        cstRemoteReader->lastSentIssueTime=getActualNtpTime();
        d->mbSend.cdrStreamDirect=&csChange->cdrStream;
        debug(52,3) ("sent: RTPS_ISSUE_BEST(0x%x) to 0x%x-0x%x\n",
                    cstRemoteReader->cstWriter->guid.oid,
                    cstRemoteReader->guid.hid,
                    cstRemoteReader->guid.aid);
        ORTESendData(d,
            cstRemoteReader->objectEntryOID->objectEntryAID,
            ORTE_FALSE);
        CSTWriterDestroyCSChangeForReader(cstRemoteReader,
            csChangeForReader,ORTE_TRUE);
        eventDetach(d,
            cstRemoteReader->objectEntryOID->objectEntryAID,
            &cstRemoteReader->delayResponceTimer,
            2);   
        //when is no csChange -> break processing 
        if (cstRemoteReader->cstWriter->csChangesCounter==0) 
          break;
        eventAdd(d,
            cstRemoteReader->objectEntryOID->objectEntryAID,
            &cstRemoteReader->delayResponceTimer,
            2,   
            "CSTWriterSendBestEffortTimer",
            CSTWriterSendBestEffortTimer,
            &cstRemoteReader->cstWriter->lock,
            cstRemoteReader,
            &sp->minimumSeparation);
        return 0;
      }
    }
  }
  cstRemoteReader->commStateSend=NOTHNIGTOSEND;
  debug(52,10) ("CSTWriterSendBestEffortTimer: finished\n");
  return 0;
}

/**********************************************************************************/
int
CSTWriterSendStrictTimer(ORTEDomain *d,void *vcstRemoteReader) {
  CSTRemoteReader   *cstRemoteReader=(CSTRemoteReader*)vcstRemoteReader;
  CSChangeForReader *csChangeForReader=NULL;
  int               max_msg_len,len;
  CSChange          *csChange;
  Boolean           firstTrace=ORTE_TRUE;
  
  debug(52,10) ("CSTWriterSendStrictTimer: start\n");
  max_msg_len=getMaxMessageLength(d);
  if (cstRemoteReader->commStateSend!=NOTHNIGTOSEND) {
    gavl_cust_for_each(CSChangeForReader,cstRemoteReader,csChangeForReader) {
      csChange=csChangeForReader->csChange;
      if (csChangeForReader->commStateChFReader==TOSEND) {
        cstRemoteReader->commStateSend=MUSTSENDDATA;
        if ((firstTrace) && (cstRemoteReader->cstWriter->params.fullAcknowledge) &&
            !d->mbSend.containsInfoReply) {
          firstTrace=ORTE_FALSE;
          len=RTPSInfoREPLYCreate(d->mbSend.cdrStream.bufferPtr,max_msg_len,
              IPADDRESS_INVALID,
              ((AppParams*)cstRemoteReader->cstWriter->objectEntryOID->attributes)->userdataUnicastPort);
          if (len<0) {
            d->mbSend.needSend=ORTE_TRUE;
            return 1;
          }
          d->mbSend.containsInfoReply=ORTE_TRUE;  
          d->mbSend.cdrStream.bufferPtr+=len;
          d->mbSend.cdrStream.length+=len;
          max_msg_len-=len;
          debug(52,3) ("sent: RTPS_InfoREPLY(0x%x) to 0x%x-0x%x\n",
                       cstRemoteReader->cstWriter->guid.oid,
                       cstRemoteReader->guid.hid,
                       cstRemoteReader->guid.aid);
        }
        len=20+cstRemoteReader->cstWriter->typeRegister->getMaxSize;
        if (max_msg_len<len) {
          d->mbSend.needSend=ORTE_TRUE;
          return 1;
        }
        memcpy(d->mbSend.cdrStream.bufferPtr,     //dest
//               csChange->cdrStream.bufferPtr-len, //src
               csChange->cdrStream.buffer+RTPS_HEADER_LENGTH+12, //src
               len);                              //length
        d->mbSend.cdrStream.bufferPtr+=len;
        d->mbSend.cdrStream.length+=len;
        max_msg_len-=len;
        debug(52,3) ("sent: RTPS_ISSUE_STRICT(0x%x) to 0x%x-0x%x\n",
                    cstRemoteReader->cstWriter->guid.oid,
                    cstRemoteReader->guid.hid,
                    cstRemoteReader->guid.aid);
      }
    }
  }
  cstRemoteReader->commStateSend=NOTHNIGTOSEND;
  debug(52,10) ("CSTWriterSendStrictTimer: finished\n");
  //add HeardBeat  
  return CSTWriterAnnounceIssueTimer(d,cstRemoteReader);
}

/**********************************************************************************/
int
CSTWriterSendTimer(ORTEDomain *d,void *vcstRemoteReader) {
  CSTRemoteReader   *cstRemoteReader=(CSTRemoteReader*)vcstRemoteReader;
  CSChangeForReader *csChangeForReader=NULL;
  unsigned int      max_msg_len;
  int               len,off;
  Boolean           firstTrace=ORTE_TRUE,f_bit=ORTE_TRUE;
  
  debug(52,10) ("CSTWriterSendTimer: start\n");
  max_msg_len=getMaxMessageLength(d);
  //setup f_bit of object
  if (cstRemoteReader->cstWriter->params.fullAcknowledge)
    f_bit=ORTE_FALSE;
  if (cstRemoteReader->commStateSend!=NOTHNIGTOSEND) {
    gavl_cust_for_each(CSChangeForReader,cstRemoteReader,csChangeForReader) {
      if (csChangeForReader->commStateChFReader==TOSEND) {
        cstRemoteReader->commStateSend=MUSTSENDDATA;
        if ((firstTrace) && (cstRemoteReader->cstWriter->params.fullAcknowledge) &&
            !d->mbSend.containsInfoReply) {
          firstTrace=ORTE_FALSE;
          len=RTPSInfoREPLYCreate(d->mbSend.cdrStream.bufferPtr,max_msg_len,
              IPADDRESS_INVALID,
              ((AppParams*)cstRemoteReader->cstWriter->objectEntryOID->attributes)->metatrafficUnicastPort);
          if (len<0) {
            d->mbSend.needSend=ORTE_TRUE;
            return 1;
          }
          d->mbSend.containsInfoReply=ORTE_TRUE;  
          d->mbSend.cdrStream.bufferPtr+=len;
          d->mbSend.cdrStream.length+=len;
          max_msg_len-=len;
          debug(52,3) ("sent: RTPS_InfoREPLY(0x%x) to 0x%x-0x%x\n",
                       cstRemoteReader->cstWriter->guid.oid,
                       cstRemoteReader->guid.hid,
                       cstRemoteReader->guid.aid);
        }
        if (max_msg_len<32) {
          d->mbSend.needSend=ORTE_TRUE;
          return 1;
        }
        off=0;
        //VAR ???
        if (SeqNumberCmp(csChangeForReader->csChange->gapSN,noneSN)==0) {
          debug(52,3) ("sent: RTPS_VAR(0x%x) to 0x%x-0x%x\n",
                       cstRemoteReader->cstWriter->guid.oid,
                       cstRemoteReader->guid.hid,
                       cstRemoteReader->guid.aid);
          len=32;
          d->mbSend.cdrStream.bufferPtr[0]=(uint8_t)VAR;
          d->mbSend.cdrStream.bufferPtr[1]=ORTE_MY_MBO;
          if (csChangeForReader->csChange->alive) 
            d->mbSend.cdrStream.bufferPtr[1]|=4;
          *((ObjectId*)(d->mbSend.cdrStream.bufferPtr+4))=OID_UNKNOWN;
          conv_u32((uint32_t*)(d->mbSend.cdrStream.bufferPtr+4),0);
          *((ObjectId*)(d->mbSend.cdrStream.bufferPtr+8))=
            cstRemoteReader->cstWriter->guid.oid;
          conv_u32((uint32_t*)(d->mbSend.cdrStream.bufferPtr+8),0);
          if (csChangeForReader->csChange->guid.oid==OID_APP) {
            d->mbSend.cdrStream.bufferPtr[1]|=8;
            *((HostId*)(d->mbSend.cdrStream.bufferPtr+12))=
              csChangeForReader->csChange->guid.hid;
            conv_u32((uint32_t*)(d->mbSend.cdrStream.bufferPtr+12),0);
            *((AppId*)(d->mbSend.cdrStream.bufferPtr+16))=
              csChangeForReader->csChange->guid.aid;
            conv_u32((uint32_t*)(d->mbSend.cdrStream.bufferPtr+16),0);
          } else {
            len-=8;
            off=-8;
          }
          *((ObjectId*)(d->mbSend.cdrStream.bufferPtr+20+off))=
            csChangeForReader->csChange->guid.oid;
          conv_u32((uint32_t*)(d->mbSend.cdrStream.bufferPtr+20+off),0);
          *((SequenceNumber*)(d->mbSend.cdrStream.bufferPtr+24+off))=
            csChangeForReader->csChange->sn;
          if (!CSChangeAttributes_is_empty(csChangeForReader->csChange)) {
            int plen;
            plen=parameterCodeStreamFromCSChange(csChangeForReader->csChange,
                 d->mbSend.cdrStream.bufferPtr+32+off,max_msg_len-len);
            if (plen<0) {
              d->mbSend.needSend=ORTE_TRUE;
              return 1;
            }
            d->mbSend.cdrStream.bufferPtr[1]|=2;
            len+=plen;
          }
        } else {  //GAP ???
          debug(52,3) ("sent: RTPS_GAP(0x%x) to 0x%x-0x%x\n",
                       cstRemoteReader->cstWriter->guid.oid,
                       cstRemoteReader->guid.hid,
                       cstRemoteReader->guid.aid);
          len=32;
          d->mbSend.cdrStream.bufferPtr[0]=(uint8_t)GAP;
          d->mbSend.cdrStream.bufferPtr[1]=ORTE_MY_MBO;
          *((ObjectId*)(d->mbSend.cdrStream.bufferPtr+4))=OID_UNKNOWN;
          conv_u32((uint32_t*)(d->mbSend.cdrStream.bufferPtr+4),0);
          *((ObjectId*)(d->mbSend.cdrStream.bufferPtr+8))=
            cstRemoteReader->cstWriter->guid.oid;
          conv_u32((uint32_t*)(d->mbSend.cdrStream.bufferPtr+8),0);
          *((SequenceNumber*)(d->mbSend.cdrStream.bufferPtr+12))=
            csChangeForReader->csChange->sn;
          conv_sn((SequenceNumber*)(d->mbSend.cdrStream.bufferPtr+12),ORTE_MY_MBO);
          SeqNumberAdd(*((SequenceNumber*)(d->mbSend.cdrStream.bufferPtr+20)),  
                       csChangeForReader->csChange->sn,
                       csChangeForReader->csChange->gapSN);
          conv_sn((SequenceNumber*)(d->mbSend.cdrStream.bufferPtr+20),ORTE_MY_MBO);
          *((uint32_t*)(d->mbSend.cdrStream.bufferPtr+28))=0;    //NumBits 
        }
        *((ParameterLength*)(d->mbSend.cdrStream.bufferPtr+2))=len-4; 
        d->mbSend.cdrStream.bufferPtr+=len;
        d->mbSend.cdrStream.length+=len;
        max_msg_len-=len;
        //setup new state for csChangeForReader
        if (NtpTimeCmp(zNtpTime,
                cstRemoteReader->cstWriter->params.waitWhileDataUnderwayTime)==0) {
          csChangeForReader->commStateChFReader=UNACKNOWLEDGED;
        } else {
          csChangeForReader->commStateChFReader=UNDERWAY;
          eventDetach(d,
              cstRemoteReader->objectEntryOID->objectEntryAID,
              &csChangeForReader->waitWhileDataUnderwayTimer,
              0);
          eventAdd(d,
              cstRemoteReader->objectEntryOID->objectEntryAID,
              &csChangeForReader->waitWhileDataUnderwayTimer,
              0,   //common timer
              "CSChangeForReaderUnderwayTimer",
              CSChangeForReaderUnderwayTimer,
              &cstRemoteReader->cstWriter->lock,
              csChangeForReader,
              &cstRemoteReader->cstWriter->params.waitWhileDataUnderwayTime);
        }
      }
    }
  }
  //add HeardBeat
  len=RTPSHeardBeatCreate(
      d->mbSend.cdrStream.bufferPtr,max_msg_len,
      &cstRemoteReader->cstWriter->firstSN,
      &cstRemoteReader->cstWriter->lastSN,
      cstRemoteReader->cstWriter->guid.oid,
      OID_UNKNOWN,
      f_bit);
  if (len<0) {
    d->mbSend.needSend=ORTE_TRUE;
    return 1;
  } else {
    //schedule new time for Announce timer
    eventDetach(d,
         cstRemoteReader->objectEntryOID->objectEntryAID,
         &cstRemoteReader->repeatAnnounceTimer,
         1);
    eventAdd(d,
         cstRemoteReader->objectEntryOID->objectEntryAID,
         &cstRemoteReader->repeatAnnounceTimer,
         1,   //metatraffic timer
         "CSTWriterAnnounceTimer",
         CSTWriterAnnounceTimer,
         &cstRemoteReader->cstWriter->lock,
         cstRemoteReader,
         &cstRemoteReader->cstWriter->params.repeatAnnounceTime);
  }
  debug(52,3) ("sent: RTPS_HB(0x%x) to 0x%x-0x%x\n",
                cstRemoteReader->cstWriter->guid.oid,
                cstRemoteReader->guid.hid,
                cstRemoteReader->guid.aid);
  if (cstRemoteReader->commStateHB==MUSTSENDHB) {
    cstRemoteReader->commStateHB=MAYSENDHB;
  }
  cstRemoteReader->commStateSend=NOTHNIGTOSEND;
  d->mbSend.cdrStream.bufferPtr+=len;
  d->mbSend.cdrStream.length+=len;
  debug(52,10) ("CSTWriterSendTimer: finished\n");
  return 0;
}
