/*
 *  $Id: ORTEPublication.c,v 0.0.0.1      2003/11/21
 *
 *  DEBUG:  section 31                  Functions working over publication
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

GAVL_CUST_NODE_INT_IMP(PublicationList, 
                       PSEntry, ObjectEntryOID, GUID_RTPS,
                       publications, psNode, guid, gavl_cmp_guid);

/*****************************************************************************/
ORTEPublication * 
ORTEPublicationCreate(ORTEDomain *d,const char *topic,const char *typeName,
    void *instance,NtpTime *persistence,int strength,
    ORTESendCallBack sendCallBack,void *sendCallBackParam,
    NtpTime *sendCallBackDelay) {
  GUID_RTPS             guid;
  CSTWriter             *cstWriter;
  CSTWriterParams       cstWriterParams;
  ORTEPublProp          *pp;
  ObjectEntryOID        *objectEntryOID;   
  CSChange              *csChange;
  TypeNode              *typeNode;
  
  debug(31,10) ("ORTEPublicationCreate: start\n");
  cstWriter=(CSTWriter*)MALLOC(sizeof(CSTWriter));
  if (!cstWriter) return NULL;
  debug(31,10) ("ORTEPublicationCreate: memory OK\n");
  pthread_rwlock_wrlock(&d->objectEntry.objRootLock);
  pthread_rwlock_wrlock(&d->objectEntry.htimRootLock);
  pthread_rwlock_rdlock(&d->typeEntry.lock);    
  if (!(typeNode=ORTEType_find(&d->typeEntry,&typeName))) {
    pthread_rwlock_unlock(&d->typeEntry.lock);    
    pthread_rwlock_unlock(&d->objectEntry.objRootLock);
    pthread_rwlock_unlock(&d->objectEntry.htimRootLock);
    printf("before call ORTEPublicationCreate is necessary to register \n\
            ser./deser. function for a given typeName!!!\n");
    return NULL;
  }  
  pthread_rwlock_wrlock(&d->publications.lock);
  //generate new guid of publisher
  d->publications.counter++;
  guid.hid=d->guid.hid;guid.aid=d->guid.aid;
  guid.oid=(d->publications.counter<<8)|OID_PUBLICATION;
  pp=(ORTEPublProp*)MALLOC(sizeof(ORTEPublProp));
  memcpy(pp,&d->domainProp.publPropDefault,sizeof(ORTEPublProp));
  strcpy(pp->topic,topic);
  strcpy(pp->typeName,typeName);
  pp->persistence=*persistence;
  pp->strength=strength;
  pp->reliabilityOffered=PID_VALUE_RELIABILITY_BEST_EFFORTS | 
                         PID_VALUE_RELIABILITY_STRICT;
  //insert object to structure objectEntry
  objectEntryOID=objectEntryAdd(d,&guid,(void*)pp);
  objectEntryOID->privateCreated=ORTE_TRUE;
  objectEntryOID->instance=instance;
  objectEntryOID->sendCallBack=sendCallBack;
  objectEntryOID->callBackParam=sendCallBackParam;
  if (objectEntryOID->sendCallBack!=NULL) {  
    if (sendCallBackDelay!=NULL) {
      objectEntryOID->sendCallBackDelay=*sendCallBackDelay;
      eventAdd(d,
          objectEntryOID->objectEntryAID,
          &objectEntryOID->sendCallBackDelayTimer,
          0,   
          "PublicationCallBackTimer",
          PublicationCallBackTimer,
          &cstWriter->lock,
          cstWriter,
          &objectEntryOID->sendCallBackDelay);               
    }
  }
  //create writerPublication
  NTPTIME_ZERO(cstWriterParams.waitWhileDataUnderwayTime);
  NTPTIME_ZERO(cstWriterParams.delayResponceTime);
  cstWriterParams.refreshPeriod=iNtpTime;  //cann't refresh csChange(s)
  cstWriterParams.repeatAnnounceTime=pp->HBNornalRate;
  cstWriterParams.HBMaxRetries=pp->HBMaxRetries;
  cstWriterParams.fullAcknowledge=ORTE_TRUE;
  CSTWriterInit(d,cstWriter,objectEntryOID,guid.oid,&cstWriterParams,
                &typeNode->typeRegister);
  //insert cstWriter to list of publications
  CSTWriter_insert(&d->publications,cstWriter);
  //generate csChange for writerPublisher
  pthread_rwlock_wrlock(&d->writerPublications.lock);
  csChange=(CSChange*)MALLOC(sizeof(CSChange));
  parameterUpdateCSChangeFromPublication(csChange,pp);
  csChange->guid=guid;
  csChange->alive=ORTE_TRUE;
  csChange->cdrStream.buffer=NULL;
  debug(31,10) ("ORTEPublicationCreate: add CSChange\n");
  CSTWriterAddCSChange(d,&d->writerPublications,csChange);
  pthread_rwlock_unlock(&d->writerPublications.lock);
  pthread_rwlock_unlock(&d->publications.lock);
  pthread_rwlock_unlock(&d->typeEntry.lock);    
  pthread_rwlock_unlock(&d->objectEntry.htimRootLock);
  pthread_rwlock_unlock(&d->objectEntry.objRootLock);
  debug(31,10) ("ORTEPublicationCreate: finished\n");
  return cstWriter;
}

/*****************************************************************************/
int
ORTEPublicationDestroy(ORTEPublication *cstWriter) {
  CSChange              *csChange;

  if (!cstWriter) return ORTE_BAD_HANDLE;
  //generate csChange for writerPublisher
  pthread_rwlock_wrlock(&cstWriter->domain->objectEntry.objRootLock);
  pthread_rwlock_wrlock(&cstWriter->domain->objectEntry.htimRootLock);
  pthread_rwlock_wrlock(&cstWriter->domain->writerPublications.lock);
  if (cstWriter->objectEntryOID->sendCallBack!=NULL) {  
    eventDetach(cstWriter->domain,
        cstWriter->objectEntryOID->objectEntryAID,
        &cstWriter->objectEntryOID->sendCallBackDelayTimer,
        0);
  }
  csChange=(CSChange*)MALLOC(sizeof(CSChange));
  CSChangeAttributes_init_head(csChange);
  csChange->cdrStream.buffer=NULL;
  csChange->guid=cstWriter->guid;
  csChange->alive=ORTE_FALSE;
  CSTWriterAddCSChange(cstWriter->domain,
                       &cstWriter->domain->writerPublications,
                       csChange);
  pthread_rwlock_unlock(&cstWriter->domain->writerPublications.lock);
  pthread_rwlock_unlock(&cstWriter->domain->objectEntry.htimRootLock);
  pthread_rwlock_unlock(&cstWriter->domain->objectEntry.objRootLock);
  return ORTE_OK;
}


/*****************************************************************************/
int
ORTEPublicationPropertiesGet(ORTEPublication *cstWriter,ORTEPublProp *pp) {
  if (!cstWriter) return ORTE_BAD_HANDLE;
  pthread_rwlock_rdlock(&cstWriter->domain->objectEntry.objRootLock);
  pthread_rwlock_rdlock(&cstWriter->lock);
  *pp=*(ORTEPublProp*)cstWriter->objectEntryOID->attributes;
  pthread_rwlock_unlock(&cstWriter->lock);
  pthread_rwlock_unlock(&cstWriter->domain->objectEntry.objRootLock);
  return ORTE_OK;
}

/*****************************************************************************/
int
ORTEPublicationPropertiesSet(ORTEPublication *cstWriter,ORTEPublProp *pp) {
  CSChange              *csChange;

  if (!cstWriter) return ORTE_BAD_HANDLE;
  pthread_rwlock_wrlock(&cstWriter->domain->objectEntry.objRootLock);
  pthread_rwlock_wrlock(&cstWriter->domain->objectEntry.htimRootLock);
  pthread_rwlock_wrlock(&cstWriter->domain->writerPublications.lock);
  pthread_rwlock_rdlock(&cstWriter->lock);
  csChange=(CSChange*)MALLOC(sizeof(CSChange));
  parameterUpdateCSChangeFromPublication(csChange,pp);
  csChange->guid=cstWriter->guid;
  csChange->alive=ORTE_TRUE;
  csChange->cdrStream.buffer=NULL;
  CSTWriterAddCSChange(cstWriter->domain,
      &cstWriter->domain->writerPublications,csChange);
  pthread_rwlock_unlock(&cstWriter->lock);
  pthread_rwlock_unlock(&cstWriter->domain->writerPublications.lock);
  pthread_rwlock_unlock(&cstWriter->domain->objectEntry.htimRootLock);
  pthread_rwlock_unlock(&cstWriter->domain->objectEntry.objRootLock);
  return ORTE_OK;
}

/*****************************************************************************/
int
ORTEPublicationWaitForSubscriptions(ORTEPublication *cstWriter,NtpTime wait,
    unsigned int retries,unsigned int noSubscriptions) {
  unsigned int rSubscriptions;
  u_int32_t sec,ms;

  if (!cstWriter) return ORTE_BAD_HANDLE;
  NtpTimeDisAssembToMs(sec,ms,wait);
  do {
    pthread_rwlock_rdlock(&cstWriter->domain->objectEntry.objRootLock);
    pthread_rwlock_rdlock(&cstWriter->lock);
    rSubscriptions=cstWriter->cstRemoteReaderCounter;
    pthread_rwlock_unlock(&cstWriter->lock);
    pthread_rwlock_unlock(&cstWriter->domain->objectEntry.objRootLock);
    if (rSubscriptions>=noSubscriptions)
      return ORTE_OK;
    ORTESleepMs(sec*1000+ms);
  } while (retries--);
  return ORTE_TIMEOUT;  
}

/*****************************************************************************/
int
ORTEPublicationGetStatus(ORTEPublication *cstWriter,ORTEPublStatus *status) {
  CSChange *csChange;

  if (!cstWriter) return ORTE_BAD_HANDLE;
  pthread_rwlock_rdlock(&cstWriter->domain->objectEntry.objRootLock);
  pthread_rwlock_rdlock(&cstWriter->lock);
  status->strict=cstWriter->strictReliableCounter;
  status->bestEffort=cstWriter->bestEffortsCounter;
  status->issues=0;
  ul_list_for_each(CSTWriterCSChange,cstWriter,csChange)
    status->issues++;
  pthread_rwlock_unlock(&cstWriter->lock);
  pthread_rwlock_unlock(&cstWriter->domain->objectEntry.objRootLock);
  return ORTE_OK;
}

/*****************************************************************************/
int
ORTEPublicationPrepareQueue(ORTEPublication *cstWriter) {
  ORTEPublProp          *pp;
  
  if (!cstWriter) return ORTE_BAD_HANDLE;
  pthread_rwlock_wrlock(&cstWriter->lock);
  pp=(ORTEPublProp*)cstWriter->objectEntryOID->attributes;
  if (cstWriter->csChangesCounter>=pp->sendQueueSize) {
    if (!CSTWriterTryDestroyBestEffortIssue(cstWriter)) {
      NtpTime             expire,atime=getActualNtpTime();
      struct timespec     wtime; 
      //Count max block time
      NtpTimeAdd(expire,atime,cstWriter->domain->domainProp.baseProp.maxBlockTime);
      NtpTimeDisAssembToUs(wtime.tv_sec,wtime.tv_nsec,expire);
      wtime.tv_nsec*=1000;  //conver to nano seconds
      pthread_rwlock_unlock(&cstWriter->lock);    
      sem_timedwait(
          &cstWriter->semCSChangeDestroyed,
          &wtime);
      pthread_rwlock_wrlock(&cstWriter->lock);    
      pp=(ORTEPublProp*)cstWriter->objectEntryOID->attributes;
      if (cstWriter->csChangesCounter>=pp->sendQueueSize) {
        debug(31,5) ("Publication: queue level (%d), queue full!!!\n",
                      cstWriter->csChangesCounter);
        pthread_rwlock_unlock(&cstWriter->lock);
        return ORTE_QUEUE_FULL;
      }
    }
  }
  pthread_rwlock_unlock(&cstWriter->lock);
  return ORTE_OK;
}

/*****************************************************************************/
int
ORTEPublicationSendLocked(ORTEPublication *cstWriter) {
  CSChange              *csChange;
  SequenceNumber        snNext;
  
  if (!cstWriter) return ORTE_BAD_HANDLE;
  pthread_rwlock_rdlock(&cstWriter->domain->typeEntry.lock);    
  pthread_rwlock_wrlock(&cstWriter->domain->writerPublications.lock);
  if (!CSTRemoteReader_is_empty(cstWriter)) {
    csChange=(CSChange*)MALLOC(sizeof(CSChange));
    CSChangeAttributes_init_head(csChange);
    csChange->guid=cstWriter->guid;
    csChange->alive=ORTE_FALSE;
    csChange->cdrStream.length=RTPS_HEADER_LENGTH+12+     //HEADER+INFO_TS+ISSUE
                              +20+cstWriter->typeRegister->getMaxSize;
    csChange->cdrStream.buffer=(u_int8_t*)MALLOC(csChange->cdrStream.length);
    csChange->cdrStream.bufferPtr=csChange->cdrStream.buffer+RTPS_HEADER_LENGTH+12+20;
    SeqNumberInc(snNext,cstWriter->lastSN);
    RTPSHeaderCreate(csChange->cdrStream.buffer,
                    cstWriter->domain->guid.hid,cstWriter->domain->guid.aid);
    RTPSInfoTSCreate(csChange->cdrStream.buffer+RTPS_HEADER_LENGTH,
                    12,getActualNtpTime());
    RTPSIssueCreateHeader(csChange->cdrStream.buffer+
                    RTPS_HEADER_LENGTH+12,20,16+cstWriter->typeRegister->getMaxSize,
                    OID_UNKNOWN,cstWriter->guid.oid,snNext);
    //serialization routine
    if (cstWriter->typeRegister->serialize) {
      cstWriter->typeRegister->serialize(
          &csChange->cdrStream,
          cstWriter->objectEntryOID->instance);
    } else {
      //no deserialization -> memcpy
      memcpy(csChange->cdrStream.bufferPtr,
            cstWriter->objectEntryOID->instance,
            cstWriter->typeRegister->getMaxSize);
      csChange->cdrStream.bufferPtr+=cstWriter->typeRegister->getMaxSize;
    }
    csChange->cdrStream.needByteSwap=ORTE_FALSE;
    CSTWriterAddCSChange(cstWriter->domain,
                        cstWriter,
                        csChange);
  }
  pthread_rwlock_unlock(&cstWriter->domain->typeEntry.lock);    
  pthread_rwlock_unlock(&cstWriter->domain->writerPublications.lock);
  return ORTE_OK;
}

/*****************************************************************************/
int
ORTEPublicationSend(ORTEPublication *cstWriter) {
  int             r;

  if (!cstWriter) return ORTE_BAD_HANDLE;
  //prepare sending queue
  if ((r=ORTEPublicationPrepareQueue(cstWriter))<0) return r;
  //send
  pthread_rwlock_wrlock(&cstWriter->domain->objectEntry.objRootLock);
  pthread_rwlock_wrlock(&cstWriter->domain->objectEntry.htimRootLock);
  pthread_rwlock_wrlock(&cstWriter->lock);
  r=ORTEPublicationSendLocked(cstWriter);
  pthread_rwlock_unlock(&cstWriter->lock);
  pthread_rwlock_unlock(&cstWriter->domain->objectEntry.htimRootLock);
  pthread_rwlock_unlock(&cstWriter->domain->objectEntry.objRootLock);
  return r;
}
