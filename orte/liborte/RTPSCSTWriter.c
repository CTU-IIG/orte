/*
 *  $Id: RTPSCSTWriter.c,v 0.0.0.1      2003/09/13 
 *
 *  DEBUG:  section 51                  CSTWriter
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

GAVL_CUST_NODE_INT_IMP(CSTWriter, 
                       CSTPublications, CSTWriter, GUID_RTPS,
                       cstWriter, node, guid, gavl_cmp_guid);
GAVL_CUST_NODE_INT_IMP(CSTRemoteReader, 
                       CSTWriter, CSTRemoteReader, GUID_RTPS,
                       cstRemoteReader, node, guid, gavl_cmp_guid);
GAVL_CUST_NODE_INT_IMP(CSChangeForReader,
                       CSTRemoteReader, CSChangeForReader, SequenceNumber,
                       csChangeForReader, node, csChange->sn, gavl_cmp_sn);

/*****************************************************************************/
void 
CSTWriterInit(ORTEDomain *d,CSTWriter *cstWriter,ObjectEntryOID *object,
              ObjectId oid,CSTWriterParams *params,ORTETypeRegister *typeRegister) {

  debug(51,10) ("CSTWriterInit: start\n");
  //init values of cstwriter
  cstWriter->guid.hid=object->objectEntryHID->hid;
  cstWriter->guid.aid=object->objectEntryAID->aid;
  cstWriter->guid.oid=oid;
  cstWriter->objectEntryOID=object;
  memcpy(&cstWriter->params,params,sizeof(CSTWriterParams));
  cstWriter->strictReliableCounter=0;
  cstWriter->bestEffortsCounter=0;
  cstWriter->csChangesCounter=0;
  cstWriter->cstRemoteReaderCounter=0;
  SEQUENCE_NUMBER_NONE(cstWriter->firstSN);
  SEQUENCE_NUMBER_NONE(cstWriter->lastSN);
  CSTWriterCSChange_init_head(cstWriter);
  CSTRemoteReader_init_root_field(cstWriter);
  pthread_rwlock_init(&cstWriter->lock,NULL);
  ul_htim_queue_init_detached(&cstWriter->refreshPeriodTimer.htim);
  cstWriter->domain=d;
  cstWriter->typeRegister=typeRegister;
  if ((cstWriter->guid.oid & 0x07)==OID_PUBLICATION) {
    pthread_cond_init(&cstWriter->condCSChangeDestroyed,NULL);
    pthread_mutex_init(&cstWriter->mutexCSChangeDestroyed,NULL);
    cstWriter->condValueCSChangeDestroyed=0;
  }
  //add event for refresh
  if (NtpTimeCmp(cstWriter->params.refreshPeriod,iNtpTime)!=0) {
    CSTWriterRefreshTimer(d,(void*)cstWriter);
  }
  debug(51,4) ("CSTWriterInit: 0x%x-0x%x-0x%x\n",
                cstWriter->guid.hid,
                cstWriter->guid.aid,
                cstWriter->guid.oid);
  debug(51,10) ("CSTWriterInit: finished\n");
}

/*****************************************************************************/
void 
CSTWriterDelete(ORTEDomain *d,CSTWriter *cstWriter) {
  CSTRemoteReader       *cstRemoteReader;
  CSChange              *csChange;

  debug(51,10) ("CSTWriterDelete: start\n");
  
  debug(51,4) ("CSTWriterDelete: 0x%x-0x%x-0x%x\n",
                cstWriter->guid.hid,
                cstWriter->guid.aid,
                cstWriter->guid.oid);
  //Destroy all cstRemoteReader connected on cstWriter
  while((cstRemoteReader=CSTRemoteReader_first(cstWriter))) {
    CSTWriterDestroyRemoteReader(d,cstRemoteReader);
  }
  //Destroy all csChnages connected on cstWriter
  while((csChange=CSTWriterCSChange_cut_first(cstWriter))) {
    parameterDelete(csChange);
    FREE(csChange);
  }
  eventDetach(d,
      cstWriter->objectEntryOID->objectEntryAID,
      &cstWriter->refreshPeriodTimer,
      0);
  if ((cstWriter->guid.oid & 0x07)==OID_PUBLICATION) {
    pthread_cond_destroy(&cstWriter->condCSChangeDestroyed);
    pthread_mutex_destroy(&cstWriter->mutexCSChangeDestroyed);
  }
  pthread_rwlock_destroy(&cstWriter->lock);
  debug(51,10) ("CSTWriterDelete: finished\n");
}

/*****************************************************************************/
void
CSTWriterAddRemoteReader(ORTEDomain *d,CSTWriter *cstWriter,ObjectEntryOID *object,
                         ObjectId oid) {
  CSTRemoteReader     *cstRemoteReader;
  CSChangeForReader   *csChangeForReader;
  CSChange            *csChange=NULL;
  
  cstWriter->cstRemoteReaderCounter++;
  cstRemoteReader=(CSTRemoteReader*)MALLOC(sizeof(CSTRemoteReader));
  cstRemoteReader->guid.hid=object->objectEntryHID->hid;
  cstRemoteReader->guid.aid=object->objectEntryAID->aid;
  cstRemoteReader->guid.oid=oid;
  cstRemoteReader->objectEntryOID=object;
  cstRemoteReader->cstWriter=cstWriter;
  CSChangeForReader_init_root_field(cstRemoteReader);
  cstRemoteReader->commStateHB=MAYSENDHB;
  cstRemoteReader->commStateSend=NOTHNIGTOSEND;
  cstRemoteReader->HBRetriesCounter=0;
  cstRemoteReader->csChangesCounter=0;
  NTPTIME_ZERO(cstRemoteReader->lastSentIssueTime);
  ul_htim_queue_init_detached(&cstRemoteReader->delayResponceTimer.htim);
  ul_htim_queue_init_detached(&cstRemoteReader->repeatAnnounceTimer.htim);
  //insert remote reader 
  CSTRemoteReader_insert(cstWriter,cstRemoteReader);
  //copy all csChanges (not for publication)
  if ((cstWriter->guid.oid & 0x07)!=OID_PUBLICATION) {
    ul_list_for_each(CSTWriterCSChange,cstWriter,csChange) {
      csChange->remoteReaderCount++;  
      cstRemoteReader->csChangesCounter++;
      csChangeForReader=(CSChangeForReader*)MALLOC(sizeof(CSChangeForReader));
      csChangeForReader->commStateChFReader=TOSEND;
      csChangeForReader->csChange=csChange;
      ul_htim_queue_init_detached(&csChangeForReader->waitWhileDataUnderwayTimer.htim);
      CSChangeForReader_insert(cstRemoteReader,csChangeForReader);
      cstRemoteReader->commStateSend=MUSTSENDDATA;
    }
    if (cstRemoteReader->commStateSend==MUSTSENDDATA) {
      eventAdd(d,
          cstRemoteReader->objectEntryOID->objectEntryAID,
          &cstRemoteReader->delayResponceTimer,
          1,   
          "CSTWriterSendTimer",
          CSTWriterSendTimer,
          &cstRemoteReader->cstWriter->lock,
          cstRemoteReader,
          &cstRemoteReader->cstWriter->params.delayResponceTime);               
    }
  } else {
    //Publication
    ORTESubsProp *sp=(ORTESubsProp*)object->attributes;
    if ((sp->reliabilityRequested & PID_VALUE_RELIABILITY_STRICT)!=0)
      cstWriter->strictReliableCounter++;
    else {
      if ((sp->reliabilityRequested & PID_VALUE_RELIABILITY_BEST_EFFORTS)!=0)
        cstWriter->bestEffortsCounter++;
    }
  }
  debug(51,4) ("CSTWriterAddRemoteReader: 0x%x-0x%x-0x%x\n",
                cstRemoteReader->guid.hid,
                cstRemoteReader->guid.aid,
                cstRemoteReader->guid.oid);
}

/*****************************************************************************/
void 
CSTWriterDestroyRemoteReader(ORTEDomain *d,CSTRemoteReader *cstRemoteReader) {
  CSChangeForReader   *csChangeForReader;
  
  if (!cstRemoteReader) return;
  cstRemoteReader->cstWriter->cstRemoteReaderCounter--;
  debug(51,4) ("CSTWriterDestroyRemoteReader: 0x%x-0x%x-0x%x\n",
                cstRemoteReader->guid.hid,
                cstRemoteReader->guid.aid,
                cstRemoteReader->guid.oid);
  if ((cstRemoteReader->cstWriter->guid.oid & 0x07)==OID_PUBLICATION) {
    ORTESubsProp *sp;
    sp=(ORTESubsProp*)cstRemoteReader->objectEntryOID->attributes;
    if ((sp->reliabilityRequested & PID_VALUE_RELIABILITY_STRICT)!=0)
      cstRemoteReader->cstWriter->strictReliableCounter--;
    else {
      if ((sp->reliabilityRequested & PID_VALUE_RELIABILITY_BEST_EFFORTS)!=0)
        cstRemoteReader->cstWriter->bestEffortsCounter--;
    }
  }  
  while((csChangeForReader=CSChangeForReader_first(cstRemoteReader))) {
    CSTWriterDestroyCSChangeForReader(cstRemoteReader,
        csChangeForReader,ORTE_TRUE);
  }
  eventDetach(d,
      cstRemoteReader->objectEntryOID->objectEntryAID,
      &cstRemoteReader->delayResponceTimer,
      1);   //metatraffic timer
  eventDetach(d,
      cstRemoteReader->objectEntryOID->objectEntryAID,
      &cstRemoteReader->delayResponceTimer,
      2);   //userdata timer
  eventDetach(d,
      cstRemoteReader->objectEntryOID->objectEntryAID,
      &cstRemoteReader->repeatAnnounceTimer,
      1);   //metatraffic timer
  eventDetach(d,
      cstRemoteReader->objectEntryOID->objectEntryAID,
      &cstRemoteReader->repeatAnnounceTimer,
      2);   //userdata timer
  CSTRemoteReader_delete(cstRemoteReader->cstWriter,cstRemoteReader);
  FREE(cstRemoteReader);
}

/*****************************************************************************/
void
CSTWriterMakeGAP(ORTEDomain *d,CSTWriter *cstWriter,GUID_RTPS *guid) {
  CSChange            *csChange,*csChange1;

  ul_list_for_each(CSTWriterCSChange,cstWriter,csChange) {
    if ((!SeqNumberCmp(csChange->gapSN,noneSN)) &&
        (!gavl_cmp_guid(&csChange->guid,guid))) {  //equal? (VAR)
      //VAR->GAP - inc gap_sn_no
      SeqNumberInc(csChange->gapSN,csChange->gapSN);  
      parameterDelete(csChange);
      //is Gap in prior or next position?
      csChange1=CSTWriterCSChange_prev(cstWriter,csChange);
      if (csChange1) {
        if (SeqNumberCmp(csChange1->gapSN,noneSN)) {
          SeqNumberAdd(csChange1->gapSN,
                       csChange1->gapSN,
                       csChange->gapSN);
          CSTWriterDestroyCSChange(d,cstWriter,csChange);
          csChange=csChange1;
        }
      }
      csChange1=CSTWriterCSChange_next(cstWriter,csChange);
      if (csChange1) {
        if (SeqNumberCmp(csChange1->gapSN,noneSN)) {
          SeqNumberAdd(csChange->gapSN,
                       csChange->gapSN,
                       csChange1->gapSN);
          CSTWriterDestroyCSChange(d,cstWriter,csChange1);
        }
      }
      break;
    }
  }
}

/*****************************************************************************/
void
CSTWriterAddCSChange(ORTEDomain *d,CSTWriter *cstWriter,CSChange *csChange) {
  CSChangeForReader   *csChangeForReader;
  CSTRemoteReader     *cstRemoteReader;
  CSChange            *csChangeFSN;
  
  debug(51,5) ("CSTWriterAddCSChange: cstWriter:0x%x-0x%x-0x%x\n",
               cstWriter->guid.hid,cstWriter->guid.aid,cstWriter->guid.oid);
  cstWriter->csChangesCounter++;
  //look for old cschange
  if ((cstWriter->guid.oid & 0x07)!=OID_PUBLICATION)
    CSTWriterMakeGAP(d,cstWriter,&csChange->guid);
  //insert cschange into database changes
  SeqNumberInc(cstWriter->lastSN,cstWriter->lastSN);
  csChange->sn=cstWriter->lastSN;
  SEQUENCE_NUMBER_NONE(csChange->gapSN);
  csChange->remoteReaderCount=cstWriter->cstRemoteReaderCounter;  
  csChange->remoteReaderBest=0;
  csChange->remoteReaderStrict=0;
  CSTWriterCSChange_insert(cstWriter,csChange);
  debug(51,5) ("CSTWriterAddCSChange: sn:0x%x\n",
               csChange->sn.low);
  //update FirstSN
  csChangeFSN=CSTWriterCSChange_first(cstWriter);
  if (SeqNumberCmp(csChangeFSN->gapSN,noneSN)>0) {
    //minimal are 2 SNs (GAP,VAR) ...
//    CSTWriterDestroyCSChange(cstWriter,csChange);
  }
  csChangeFSN=CSTWriterCSChange_first(cstWriter);
  cstWriter->firstSN=csChangeFSN->sn;
  //insert new cschange for each reader
  gavl_cust_for_each(CSTRemoteReader,cstWriter,cstRemoteReader) {
    //csChangeForReader
    csChangeForReader=(CSChangeForReader*)MALLOC(sizeof(CSChangeForReader));
    csChangeForReader->commStateChFReader=TOSEND;
    csChangeForReader->csChange=csChange;
    ul_htim_queue_init_detached(&csChangeForReader->waitWhileDataUnderwayTimer.htim);
    CSChangeForReader_insert(cstRemoteReader,csChangeForReader);
    cstRemoteReader->csChangesCounter++;
    cstRemoteReader->HBRetriesCounter=0;
    if (cstRemoteReader->commStateSend==NOTHNIGTOSEND) {
      cstRemoteReader->commStateSend=MUSTSENDDATA;
      if ((cstWriter->guid.oid & 0x07)!=OID_PUBLICATION) {
        eventDetach(d,
            cstRemoteReader->objectEntryOID->objectEntryAID,
            &cstRemoteReader->delayResponceTimer,
            1);
        eventAdd(d,
            cstRemoteReader->objectEntryOID->objectEntryAID,
            &cstRemoteReader->delayResponceTimer,
            1,   
            "CSTWriterSendTimer",
            CSTWriterSendTimer,
            &cstRemoteReader->cstWriter->lock,
            cstRemoteReader,
            NULL);
      } else {
        ORTESubsProp *sp=(ORTESubsProp*)cstRemoteReader->objectEntryOID->attributes;
        
        if ((sp->reliabilityRequested & PID_VALUE_RELIABILITY_STRICT)!=0) {
          //Strict reliable subscription
          csChange->remoteReaderStrict++;
          eventDetach(d,
              cstRemoteReader->objectEntryOID->objectEntryAID,
              &cstRemoteReader->delayResponceTimer,
              2);
          eventAdd(d,
              cstRemoteReader->objectEntryOID->objectEntryAID,
              &cstRemoteReader->delayResponceTimer,
              2,   
              "CSTWriterSendStrictTimer",
              CSTWriterSendStrictTimer,
              &cstRemoteReader->cstWriter->lock,
              cstRemoteReader,
              NULL);
        } else {
          if ((sp->reliabilityRequested & PID_VALUE_RELIABILITY_BEST_EFFORTS)!=0) {
            //best efforts subscription
            NtpTime nextIssueTime,nextIssueDelay,actTime=getActualNtpTime();
            
            csChange->remoteReaderBest++;
            NtpTimeAdd(nextIssueTime,
                      cstRemoteReader->lastSentIssueTime,
                      sp->minimumSeparation);
            NtpTimeSub(nextIssueDelay,
                      nextIssueTime,
                      actTime);
            if (NtpTimeCmp(actTime,nextIssueTime)>=0) 
              NTPTIME_ZERO(nextIssueDelay);
            eventDetach(d,
                cstRemoteReader->objectEntryOID->objectEntryAID,
                &cstRemoteReader->delayResponceTimer,
                2);
            if (NtpTimeCmp(nextIssueDelay,zNtpTime)==0) {
              //direct sent issue, for case zero time
              CSTWriterSendBestEffortTimer(d,(void*)cstRemoteReader);
            } else {
              //schedule sent issue (future)
              eventAdd(d,
                  cstRemoteReader->objectEntryOID->objectEntryAID,
                  &cstRemoteReader->delayResponceTimer,
                  2,   
                  "CSTWriterSendBestEffortTimer",
                  CSTWriterSendBestEffortTimer,
                  &cstRemoteReader->cstWriter->lock,
                  cstRemoteReader,
                  &nextIssueDelay);
            }
          } else {
            //!Best_Effort & !Strict_Reliable
            CSTWriterDestroyCSChangeForReader(cstRemoteReader,csChangeForReader,
              ORTE_TRUE);
            debug(51,5) ("CSTWriterAddCSChange: destryed\n");
	      
          }
        }
      }
    }
    debug(51,5) ("CSTWriterAddCSChange: scheduled Var | Gap | Issue | HB \n");
  }
  debug(51,5) ("CSTWriterAddCSChange: finished\n");
}

/*****************************************************************************/
void 
CSTWriterDestroyCSChangeForReader(CSTRemoteReader *cstRemoteReader,
    CSChangeForReader   *csChangeForReader,Boolean destroyCSChange) {
  CSChange *csChange;
  if (!csChangeForReader) return;
  csChange=csChangeForReader->csChange;
  csChange->remoteReaderCount--;  
  cstRemoteReader->csChangesCounter--;
  if (!cstRemoteReader->csChangesCounter) {
    cstRemoteReader->commStateSend=NOTHNIGTOSEND;
  }
  if ((cstRemoteReader->cstWriter->guid.oid & 0x07)==OID_PUBLICATION) {
    ORTESubsProp *sp=(ORTESubsProp*)cstRemoteReader->objectEntryOID->attributes;
    if ((sp->reliabilityRequested & PID_VALUE_RELIABILITY_STRICT)!=0) {
        csChange->remoteReaderStrict--;
    } else {
      if ((sp->reliabilityRequested & PID_VALUE_RELIABILITY_BEST_EFFORTS)!=0) {
        csChange->remoteReaderBest--;
      }
    }
  }  
  eventDetach(cstRemoteReader->cstWriter->domain,
      cstRemoteReader->objectEntryOID->objectEntryAID,
      &csChangeForReader->waitWhileDataUnderwayTimer,
      0);
  CSChangeForReader_delete(cstRemoteReader,csChangeForReader);
  FREE(csChangeForReader);
  if ((cstRemoteReader->cstWriter->guid.oid & 0x07)==OID_PUBLICATION) {
    if (!csChange->remoteReaderCount) {
      if (destroyCSChange) {
        CSTWriterDestroyCSChange(cstRemoteReader->cstWriter->domain,
            cstRemoteReader->cstWriter,csChange);
      }
      pthread_mutex_lock(&cstRemoteReader->cstWriter->mutexCSChangeDestroyed);
      cstRemoteReader->cstWriter->condValueCSChangeDestroyed=1;
      pthread_cond_signal(&cstRemoteReader->cstWriter->condCSChangeDestroyed);
      pthread_mutex_unlock(&cstRemoteReader->cstWriter->mutexCSChangeDestroyed);
      debug(51,5) ("Publication: new queue level (%d)\n",
                  cstRemoteReader->cstWriter->csChangesCounter);
    }
  }
}

/*****************************************************************************/
void 
CSTWriterDestroyCSChange(ORTEDomain *d,CSTWriter *cstWriter,CSChange *csChange) {
  CSTRemoteReader     *cstRemoteReader;
  CSChangeForReader   *csChangeForReader;
  CSChange            *csChangeFSN;

  if (!csChange) return;
  cstWriter->csChangesCounter--;
  CSTWriterCSChange_delete(cstWriter,csChange);
  gavl_cust_for_each(CSTRemoteReader,cstWriter,cstRemoteReader) {
    csChangeForReader=CSChangeForReader_find(cstRemoteReader,&csChange->sn);
    CSTWriterDestroyCSChangeForReader(cstRemoteReader,
        csChangeForReader,ORTE_FALSE);
  }
  if (csChange->cdrStream.buffer)
    FREE(csChange->cdrStream.buffer);
  parameterDelete(csChange);
  FREE(csChange);
  //update first SN
  csChangeFSN=CSTWriterCSChange_first(cstWriter);
  if (csChangeFSN)
    cstWriter->firstSN=csChangeFSN->sn;
  else
    cstWriter->firstSN=cstWriter->lastSN;
}

/*****************************************************************************/
Boolean
CSTWriterTryDestroyBestEffortIssue(CSTWriter *cstWriter) {
  CSChange *csChange;
  ul_list_for_each(CSTWriterCSChange,cstWriter,csChange) {
    if (!csChange->remoteReaderStrict) {
      CSTWriterDestroyCSChange(cstWriter->domain,cstWriter,csChange);
      return ORTE_TRUE;
    }
  }
  return ORTE_FALSE;
}

/*****************************************************************************/
void
CSTWriterRefreshAllCSChanges(ORTEDomain *d,CSTRemoteReader *cstRemoteReader) {
  CSChangeForReader   *csChangeForReader;
  int32_t             timerQueue=1;
  
  if ((cstRemoteReader->cstWriter->guid.oid & 0x07)==OID_PUBLICATION)
    timerQueue=2; //userdata timer queue
  gavl_cust_for_each(CSChangeForReader,cstRemoteReader,csChangeForReader) {
    //refresh only VAR's
    if (SeqNumberCmp(csChangeForReader->csChange->gapSN,noneSN)==0) { 
      csChangeForReader->commStateChFReader=TOSEND;
      if (cstRemoteReader->commStateSend==NOTHNIGTOSEND) {
        cstRemoteReader->commStateSend=MUSTSENDDATA;
        eventDetach(d,
            cstRemoteReader->objectEntryOID->objectEntryAID,
            &cstRemoteReader->delayResponceTimer,
            timerQueue);
        eventAdd(d,
            cstRemoteReader->objectEntryOID->objectEntryAID,
            &cstRemoteReader->delayResponceTimer,
            timerQueue,  
            "CSTWriterSendTimer",
            CSTWriterSendTimer,
            &cstRemoteReader->cstWriter->lock,
            cstRemoteReader,
            &cstRemoteReader->cstWriter->params.delayResponceTime);               
      }
    }
  }
}
