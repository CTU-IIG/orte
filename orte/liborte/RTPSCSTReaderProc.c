/*
 *  $Id: RTPSCSTReaderProc.c,v 0.0.0.1 2003/09/13 
 *
 *  DEBUG:  section 54                  CSChanges processing
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
CSTReaderProcCSChangesManager(ORTEDomain *d,CSTRemoteWriter *cstRemoteWriter,
    CSChangeFromWriter *csChangeFromWriter) {
  CSChange           *csChange;
  ObjectEntryOID     *objectEntryOID;

  
  csChange=csChangeFromWriter->csChange;
  objectEntryOID=objectEntryFind(d,&csChangeFromWriter->csChange->guid);
  if (!objectEntryOID) return;
  if (!csChange->alive) {
    eventDetach(d,
            objectEntryOID->objectEntryAID,
            &objectEntryOID->expirationPurgeTimer,
            0);
    eventAdd(d,
            objectEntryOID->objectEntryAID,
            &objectEntryOID->expirationPurgeTimer,
            0,
            "ExpirationTimer",
            objectEntryExpirationTimer,
            NULL,
            objectEntryOID,
            NULL);
   return;
  }
  switch (csChange->guid.aid & 0x03) {
    case MANAGER:
      //update parameters of object
      parameterUpdateApplication(csChange,(AppParams*)objectEntryOID->attributes);
      //copy csChange to writerManagers
      CSTReaderDestroyCSChangeFromWriter(cstRemoteWriter,
                               csChangeFromWriter,
                               ORTE_TRUE);
      pthread_rwlock_wrlock(&d->writerManagers.lock);
      CSTWriterAddCSChange(d,&d->writerManagers,csChange);
      pthread_rwlock_unlock(&d->writerManagers.lock);
    break;
    case MANAGEDAPPLICATION: 
      //update parameters of object
      parameterUpdateApplication(csChange,(AppParams*)objectEntryOID->attributes);
      //changes can make only local Apps
      if (cstRemoteWriter->objectEntryOID->appMOM) {
        CSTReaderDestroyCSChangeFromWriter(cstRemoteWriter,
                                 csChangeFromWriter,
                                 ORTE_TRUE);
        pthread_rwlock_wrlock(&d->writerApplications.lock);
        CSTWriterAddCSChange(d,&d->writerApplications,csChange);
        pthread_rwlock_unlock(&d->writerApplications.lock);
      }
    break;
  }
}

/*****************************************************************************/
void 
CSTReaderProcCSChangesApp(ORTEDomain *d,CSTRemoteWriter *cstRemoteWriter,
    CSChangeFromWriter *csChangeFromWriter) {
  CSChange           *csChange;
  ObjectEntryOID     *objectEntryOID;
    
  csChange=csChangeFromWriter->csChange;
  objectEntryOID=objectEntryFind(d,&csChangeFromWriter->csChange->guid);
  if (!objectEntryOID) return;
  if (!csChange->alive) {
    eventDetach(d,
            objectEntryOID->objectEntryAID,
            &objectEntryOID->expirationPurgeTimer,
            0);
    eventAdd(d,
            objectEntryOID->objectEntryAID,
            &objectEntryOID->expirationPurgeTimer,
            0,
            "ExpirationTimer",
            objectEntryExpirationTimer,
            NULL,
            objectEntryOID,
            NULL);
    return;
  }
  switch (csChangeFromWriter->csChange->guid.oid & 0x07) {
    case OID_APPLICATION:
      break;
    case OID_PUBLICATION:      
      break;
    case OID_SUBSCRIPTION: 
      break;
  }
}

/*****************************************************************************/
void 
CSTReaderProcCSChanges(ORTEDomain *d,CSTRemoteWriter *cstRemoteWriter) {
  CSChangeFromWriter *csChangeFromWriter;
  SequenceNumber     snNext;

  debug(54,10) ("CSTReaderProcCSChanges: start\n");
  if (!cstRemoteWriter) return;
  while (1) {
    csChangeFromWriter=CSChangeFromWriter_first(cstRemoteWriter);
    if (!csChangeFromWriter) break;
    if (SeqNumberCmp(csChangeFromWriter->csChange->sn,
                     cstRemoteWriter->firstSN)>=0) {
      SeqNumberInc(snNext,cstRemoteWriter->sn);
      debug(54,10) ("CSTReaderProcCSChanges: processing sn:%u,Change sn:%u\n",snNext.low,
                                             csChangeFromWriter->csChange->sn.low);
      if ((SeqNumberCmp(csChangeFromWriter->csChange->sn,snNext)==0) &&
          (csChangeFromWriter->commStateChFWriter==RECEIVED)) {
        if (SeqNumberCmp(csChangeFromWriter->csChange->gapSN,noneSN)==0) {
          if ((d->guid.aid & 0x03)==MANAGER) 
              CSTReaderProcCSChangesManager(d,cstRemoteWriter,
                                            csChangeFromWriter);
          if ((d->guid.aid & 0x03)==MANAGEDAPPLICATION) 
              CSTReaderProcCSChangesApp(d,cstRemoteWriter,
                                        csChangeFromWriter);
          SeqNumberInc(cstRemoteWriter->sn,cstRemoteWriter->sn);
        } else {
          //GAP
          SeqNumberAdd(cstRemoteWriter->sn,
                      cstRemoteWriter->sn,
                      csChangeFromWriter->csChange->gapSN);
        }
        CSTReaderDestroyCSChange(cstRemoteWriter,  //note:csChange can be coped to another CSTWriter!!!
            &snNext,ORTE_FALSE);
      } else
        break;
    } else {
      CSTReaderDestroyCSChangeFromWriter(cstRemoteWriter,
          csChangeFromWriter,ORTE_FALSE);
    }
  }
  CSTReaderSetupState(cstRemoteWriter);    
  debug(54,10) ("CSTReaderProcCSChanges: finished\n");
}

/*****************************************************************************/
void 
CSTReaderProcCSChangesIssue(CSTRemoteWriter *cstRemoteWriter,Boolean pullCalled) {
  ORTESubsProp         *sp;
  CSChangeFromWriter   *csChangeFromWriter;
  ORTERecvInfo         info;  
 
  debug(54,10) ("CSTReaderProcIssue: start\n");
  if (cstRemoteWriter==NULL) return;
  sp=(ORTESubsProp*)cstRemoteWriter->cstReader->objectEntryOID->attributes;
  //Strict
  if ((sp->reliabilityRequested & PID_VALUE_RELIABILITY_STRICT)!=0) {
  } else {
    //Best Effort
    if ((sp->reliabilityRequested & PID_VALUE_RELIABILITY_BEST_EFFORTS)!=0) {
      if ((cstRemoteWriter!=
           cstRemoteWriter->cstReader->cstRemoteWriterSubscribed) ||
          (cstRemoteWriter->cstReader->cstRemoteWriterSubscribed==NULL)) 
        return;
      if ((sp->mode==PULLED) && (pullCalled==ORTE_FALSE)) return;
      while((csChangeFromWriter=CSChangeFromWriter_first(cstRemoteWriter))) {
        ObjectEntryOID *objectEntryOID;
        objectEntryOID=cstRemoteWriter->cstReader->objectEntryOID;
        if (objectEntryOID->recvCallBack) {
          //deserialization routine
          if (cstRemoteWriter->cstReader->typeRegister->deserialize) {
            cstRemoteWriter->cstReader->typeRegister->deserialize(
                &csChangeFromWriter->csChange->cdrStream,
                objectEntryOID->instance);
          } else {
            int length=csChangeFromWriter->csChange->cdrStream.length;
            if (cstRemoteWriter->cstReader->typeRegister->getMaxSize<length)
              length=cstRemoteWriter->cstReader->typeRegister->getMaxSize;
            //no deserialization -> memcpy
            memcpy(objectEntryOID->instance,
                   csChangeFromWriter->csChange->cdrStream.buffer,
                   length);
          }
          info.status=NEW_DATA;
          info.topic=sp->topic;
          info.type=sp->typeName;
          info.senderGUID=csChangeFromWriter->csChange->guid;
          info.localTimeReceived=csChangeFromWriter->csChange->localTimeReceived;
          info.remoteTimePublished=csChangeFromWriter->csChange->remoteTimePublished;
          info.sn=csChangeFromWriter->csChange->sn;
          objectEntryOID->recvCallBack(&info,
                                   objectEntryOID->instance,
                                   objectEntryOID->callBackParam);
          if (sp->mode==IMMEDIATE) {
            //setup new time for deadline timer
            eventDetach(cstRemoteWriter->cstReader->domain,
                cstRemoteWriter->cstReader->objectEntryOID->objectEntryAID,
                &cstRemoteWriter->cstReader->deadlineTimer,
                0);
            eventAdd(cstRemoteWriter->cstReader->domain,
                cstRemoteWriter->cstReader->objectEntryOID->objectEntryAID,
                &cstRemoteWriter->cstReader->deadlineTimer,
                0,   //common timer
                "CSTReaderDeadlineTimer",
                CSTReaderDeadlineTimer,
                &cstRemoteWriter->cstReader->lock,
                cstRemoteWriter->cstReader,
                &sp->deadline);
          }
          if (sp->mode==PULLED) {
            NtpTime timeNext;
            NtpTimeAdd(timeNext,
                      (getActualNtpTime()),
                      sp->deadline);
            htimerUnicastCommon_set_expire(&cstRemoteWriter->
                      cstReader->deadlineTimer,timeNext);
          }
        }
        CSTReaderDestroyCSChangeFromWriter(
            cstRemoteWriter,
            csChangeFromWriter,
            ORTE_FALSE);
      }
    }
  }  
  CSTReaderSetupState(cstRemoteWriter);    
  debug(54,10) ("CSTReaderProcIssue: finished\n");
}
