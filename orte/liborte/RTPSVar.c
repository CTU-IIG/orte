/*                            
 *  $Id: RTPSVar.c,v 0.0.0.1            2003/10/07 
 *
 *  DEBUG:  section 46                  RTPS message VAR
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

void NewPublisher(ORTEDomain *d,ObjectEntryOID *op);
void NewSubscriber(ORTEDomain *d,ObjectEntryOID *os);

/**********************************************************************************/
void 
RTPSVar(ORTEDomain *d,uint8_t *rtps_msg,MessageInterpret *mi,IPAddress senderIPAddress) {
  GUID_RTPS          objectGUID,writerGUID;
  ObjectId           roid,woid;
  SequenceNumber     sn;   
  int8_t             e_bit,p_bit,a_bit;
  uint16_t           submsg_len;
  CSTReader          *cstReader=NULL;
  CSTRemoteWriter    *cstRemoteWriter=NULL;
  CSTRemoteReader    *cstRemoteReader=NULL;
  CSChange           *csChange;
  ObjectEntryAID     *objectEntryAID;
  ObjectEntryOID     *objectEntryOID;

  e_bit=rtps_msg[1] & 0x01;
  p_bit=(rtps_msg[1] & 0x02)>>1;
  a_bit=(rtps_msg[1] & 0x04)>>2;
  submsg_len=*((uint16_t*)(rtps_msg+2));
  conv_u16(&submsg_len,e_bit);
  roid=*((ObjectId*)(rtps_msg+4));              /* readerObjectId */
  conv_u32(&roid,0);
  woid=*((ObjectId*)(rtps_msg+8));              /* writerObjectId */
  conv_u32(&woid,0);
  if (rtps_msg[1] & 0x08) {                     /* bit H          */
    objectGUID.hid=*((HostId*)(rtps_msg+12));      /* HostId         */
    conv_u32(&objectGUID.hid,0);
    objectGUID.aid=*((AppId*)(rtps_msg+16));       /* AppId          */
    conv_u32(&objectGUID.aid,0);
  } else {
    rtps_msg-=8;
    objectGUID.hid=mi->sourceHostId;
    objectGUID.aid=mi->sourceAppId;
  }	
  objectGUID.oid=*((ObjectId*)(rtps_msg+20));      /* ObjectId       */
  conv_u32(&objectGUID.oid,0);
  sn=*((SequenceNumber*)(rtps_msg+24));         /* writerSN       */
  conv_sn(&sn,e_bit);
  writerGUID.hid=mi->sourceHostId;
  writerGUID.aid=mi->sourceAppId;
  writerGUID.oid=woid;

  debug(46,3) ("recv: RTPS_VAR(0x%x) from 0x%x-0x%x sn:%u\n",
                woid,mi->sourceHostId,mi->sourceAppId,sn.low);
  
  //prepare csChange
  csChange=(CSChange*)MALLOC(sizeof(CSChange));
  csChange->cdrStream.buffer=NULL;
  csChange->guid=objectGUID;
  if (a_bit) csChange->alive=ORTE_TRUE;
  else csChange->alive=ORTE_FALSE;
  if (p_bit)
    parameterDecodeStreamToCSChange(csChange,rtps_msg+32,submsg_len,e_bit);
  else
    CSChangeAttributes_init_head(csChange);
  csChange->sn=sn;
  SEQUENCE_NUMBER_NONE(csChange->gapSN);

  //Manager
  if ((d->guid.aid & 0x03)==MANAGER) {
    if ((writerGUID.oid==OID_WRITE_APPSELF) && 
        ((writerGUID.aid & 0x03)==MANAGER)) {
      //readerManagers
      pthread_rwlock_wrlock(&d->readerManagers.lock);
      pthread_rwlock_wrlock(&d->writerApplicationSelf.lock);
      cstReader=&d->readerManagers;
      gavl_cust_for_each(CSTRemoteReader,
                         &d->writerApplicationSelf,
                         cstRemoteReader) {
        if (cstRemoteReader->guid.hid==senderIPAddress) break;
      }
      if (cstRemoteReader) {
        objectEntryOID=objectEntryFind(d,&objectGUID);
        if (!objectEntryOID) {
          //  create new RemoteReader
          AppParams *ap=(AppParams*)MALLOC(sizeof(AppParams));
          AppParamsInit(ap);
          parameterUpdateApplication(csChange,ap);
          if (generateEvent(d,&objectGUID,(void*)ap,ORTE_TRUE) &&
              csChange->alive) {
            debug(46,2) ("manager 0x%x-0x%x accepted\n",
                          objectGUID.hid,objectGUID.aid);
            objectEntryOID=objectEntryAdd(d,&objectGUID,(void*)ap);          
            CSTWriterRefreshAllCSChanges(d,cstRemoteReader);
            CSTReaderAddRemoteWriter(d,cstReader,objectEntryOID,writerGUID.oid);
            pthread_rwlock_wrlock(&d->readerApplications.lock);
            pthread_rwlock_wrlock(&d->writerApplications.lock);
            CSTReaderAddRemoteWriter(d,&d->readerApplications,objectEntryOID,OID_WRITE_APP);
            CSTWriterAddRemoteReader(d,&d->writerApplications,objectEntryOID,OID_READ_APP);
            pthread_rwlock_unlock(&d->writerApplications.lock);
            pthread_rwlock_unlock(&d->readerApplications.lock);
            //all applications from manager node set expiration timer
            gavl_cust_for_each(ObjectEntryAID,
                               objectEntryOID->objectEntryHID,objectEntryAID) {
              ObjectEntryOID *objectEntryOID1;
              objectEntryOID1=ObjectEntryOID_find(objectEntryAID,&objectGUID.oid);
              objectEntryRefreshApp(d,objectEntryOID1);
            }
          } else {
            FREE(ap);
          }
        }
        cstRemoteWriter=CSTRemoteWriter_find(cstReader,&writerGUID);
        if (cstRemoteWriter)
          objectEntryRefreshApp(d,cstRemoteWriter->objectEntryOID);
      } else {
        //deny Manager
      }
      pthread_rwlock_unlock(&d->writerApplicationSelf.lock);
    }
    if (((writerGUID.oid==OID_WRITE_APPSELF) &&
         ((writerGUID.aid & 0x03)==MANAGEDAPPLICATION)) ||
        ((writerGUID.oid==OID_WRITE_APP) &&
         ((writerGUID.aid & 0x03)==MANAGER))) {
      //readerApplication
      pthread_rwlock_wrlock(&d->readerApplications.lock);
      cstReader=&d->readerApplications;
      objectEntryOID=objectEntryFind(d,&objectGUID);
      if (!objectEntryOID) {
        AppParams *ap=(AppParams*)MALLOC(sizeof(AppParams));
        AppParamsInit(ap);
        parameterUpdateApplication(csChange,ap);
        if (generateEvent(d,&objectGUID,(void*)ap,ORTE_TRUE) &&
            csChange->alive) {
          objectEntryOID=objectEntryAdd(d,&objectGUID,(void*)ap);
          objectEntryOID->appMOM=getTypeApp(d,ap,senderIPAddress);
          if (objectEntryOID->appMOM) {
            debug(46,2) ("MOM application 0x%x-0x%x accepted\n",
                          objectGUID.hid,objectGUID.aid);
            //increment vargAppsSequenceNumber and make csChange
            SeqNumberInc(d->appParams->vargAppsSequenceNumber,
                         d->appParams->vargAppsSequenceNumber);
            //WAS & WM is locked inside next function
            appSelfParamChanged(d,ORTE_TRUE,ORTE_FALSE,ORTE_TRUE,ORTE_TRUE);
            CSTReaderAddRemoteWriter(d,cstReader,
                objectEntryOID,writerGUID.oid);
            CSTWriterAddRemoteReader(d,&d->writerManagers,
                objectEntryOID,OID_READ_MGR);
            pthread_rwlock_unlock(&d->writerApplicationSelf.lock);
            pthread_rwlock_unlock(&d->writerManagers.lock);
          } else {
            debug(46,2) ("OAM application 0x%x-0x%x accepted\n",
                          objectGUID.hid,objectGUID.aid);
          }
          pthread_rwlock_wrlock(&d->writerApplications.lock);
          CSTWriterAddRemoteReader(d,&d->writerApplications,objectEntryOID,OID_READ_APP);
          pthread_rwlock_unlock(&d->writerApplications.lock);
        } else {
          FREE(ap);
        }        
      }
      if (objectEntryOID) {
        cstRemoteWriter=CSTRemoteWriter_find(cstReader,&writerGUID);
        if (objectEntryOID->appMOM) {
          objectEntryRefreshApp(d,objectEntryOID);
        } else {
          //turn off expiration timer
          eventDetach(d,
              objectEntryOID->objectEntryAID,
              &objectEntryOID->expirationPurgeTimer,
              0);
          debug(46,3) ("for application 0x%x-0x%x turn off expiration timer\n",
                        objectEntryOID->guid.hid,objectEntryOID->guid.aid);
        }
      }
    }
  }  
  //ManagedApplication
  if ((d->guid.aid & 3)==MANAGEDAPPLICATION) {
    switch (writerGUID.oid) {
      case OID_WRITE_MGR:
        pthread_rwlock_wrlock(&d->readerManagers.lock);        
        cstReader=&d->readerManagers;
        objectEntryOID=objectEntryFind(d,&objectGUID);
        if (!objectEntryOID) {
          AppParams *ap=(AppParams*)MALLOC(sizeof(AppParams));
          AppParamsInit(ap);
          parameterUpdateApplication(csChange,ap);
          if (generateEvent(d,&objectGUID,(void*)ap,ORTE_TRUE) &&
              csChange->alive) {
            debug(46,2) ("new manager 0x%x-0x%x accepted\n",
                          objectGUID.hid,objectGUID.aid);
            objectEntryOID=objectEntryAdd(d,&objectGUID,(void*)ap);
            objectEntryOID->privateCreated=ORTE_FALSE;
            pthread_rwlock_wrlock(&d->readerApplications.lock);
            CSTReaderAddRemoteWriter(d,&d->readerApplications,
                                    objectEntryOID,OID_WRITE_APP);
            pthread_rwlock_unlock(&d->readerApplications.lock);
            //all applications from manager node set expiration timer
            gavl_cust_for_each(ObjectEntryAID,
                               objectEntryOID->objectEntryHID,objectEntryAID) {
              ObjectEntryOID *objectEntryOID1;
              objectEntryOID1=ObjectEntryOID_find(objectEntryAID,&objectGUID.oid);
              objectEntryRefreshApp(d,objectEntryOID1);
            }
          } else {
            FREE(ap);
          }
        } else {
          GUID_RTPS guid_wapp=objectGUID;
          guid_wapp.oid=OID_WRITE_APP;
          pthread_rwlock_wrlock(&d->readerApplications.lock);
          cstRemoteWriter=CSTRemoteWriter_find(&d->readerApplications,&guid_wapp);
          //setup state of cstRemoteWriter on send ACK to manager
          if (cstRemoteWriter) {
            if (cstRemoteWriter->commStateACK==WAITING) {
              eventDetach(d,
                  cstRemoteWriter->objectEntryOID->objectEntryAID,
                  &cstRemoteWriter->repeatActiveQueryTimer,
                  1);   //metatraffic timer
              eventAdd(d,
                  cstRemoteWriter->objectEntryOID->objectEntryAID,
                  &cstRemoteWriter->repeatActiveQueryTimer,
                  1,   //metatraffic timer
                  "CSTReaderQueryTimer",
                  CSTReaderQueryTimer,
                  &cstRemoteWriter->cstReader->lock,
                  cstRemoteWriter,
                  NULL);               
            }
          }
          pthread_rwlock_unlock(&d->readerApplications.lock);
        } 
        objectEntryRefreshApp(d,objectEntryOID);
        cstRemoteWriter=CSTRemoteWriter_find(cstReader,&writerGUID);
        if ((!cstRemoteWriter) &&
            (objectGUID.hid==writerGUID.hid) && (objectGUID.aid==writerGUID.aid)) {
          cstRemoteWriter=
              CSTReaderAddRemoteWriter(d,cstReader,objectEntryOID,writerGUID.oid);
        }
        break;
      case OID_WRITE_APP:
        pthread_rwlock_wrlock(&d->readerApplications.lock);        
        cstReader=&d->readerApplications;
        cstRemoteWriter=CSTRemoteWriter_find(cstReader,&writerGUID);
        if (cstRemoteWriter) {
          GUID_RTPS guid_tmp=objectGUID;
          guid_tmp.oid=OID_WRITE_PUBL;
          objectEntryOID=objectEntryFind(d,&objectGUID);
          if (!CSTRemoteWriter_find(&d->readerPublications,&guid_tmp)) {
            if (!objectEntryOID) {
              AppParams *ap=(AppParams*)MALLOC(sizeof(AppParams));
              AppParamsInit(ap);
              parameterUpdateApplication(csChange,ap);
              if (generateEvent(d,&objectGUID,(void*)ap,ORTE_TRUE) &&
                  csChange->alive) {
                debug(46,2) ("new application 0x%x-0x%x accepted\n",
                              objectGUID.hid,objectGUID.aid);
                objectEntryOID=objectEntryAdd(d,&objectGUID,(void*)ap);
                objectEntryOID->privateCreated=ORTE_FALSE;
              } else {
                FREE(ap);
                break;
              }
            }
            pthread_rwlock_wrlock(&d->readerPublications.lock);            
            pthread_rwlock_wrlock(&d->readerSubscriptions.lock);            
            pthread_rwlock_wrlock(&d->writerPublications.lock);            
            pthread_rwlock_wrlock(&d->writerSubscriptions.lock);            
            CSTReaderAddRemoteWriter(d,&d->readerPublications,
                                     objectEntryOID,OID_WRITE_PUBL);
            CSTReaderAddRemoteWriter(d,&d->readerSubscriptions,
                                     objectEntryOID,OID_WRITE_SUBS);
            CSTWriterAddRemoteReader(d,&d->writerPublications,
                                     objectEntryOID,OID_READ_PUBL);
            CSTWriterAddRemoteReader(d,&d->writerSubscriptions,
                                     objectEntryOID,OID_READ_SUBS);
            pthread_rwlock_unlock(&d->readerPublications.lock);            
            pthread_rwlock_unlock(&d->readerSubscriptions.lock);            
            pthread_rwlock_unlock(&d->writerPublications.lock);            
            pthread_rwlock_unlock(&d->writerSubscriptions.lock);            
          }
          if (objectEntryOID) {
            //turn off expiration timer
            eventDetach(d,
                objectEntryOID->objectEntryAID,
                &objectEntryOID->expirationPurgeTimer,
                0);
            debug(46,3) ("for application 0x%x-0x%x turn off expiration timer\n",
                          objectEntryOID->guid.hid,
                          objectEntryOID->guid.aid);
          }
        }
        break;
      case OID_WRITE_PUBL:
        pthread_rwlock_wrlock(&d->readerPublications.lock);        
        cstReader=&d->readerPublications;
        cstRemoteWriter=CSTRemoteWriter_find(cstReader,&writerGUID);
        if (cstRemoteWriter) {
          objectEntryOID=objectEntryFind(d,&objectGUID);
          if (!objectEntryOID) {
            ORTEPublProp *pp=(ORTEPublProp*)MALLOC(sizeof(ORTEPublProp));
            PublParamsInit(pp);
            parameterUpdatePublication(csChange,pp);
            if (generateEvent(d,&objectGUID,(void*)pp,ORTE_TRUE) &&
                csChange->alive) {
              debug(46,2) ("new publisher 0x%x-0x%x-0x%x accepted\n",
                            objectGUID.hid,objectGUID.aid,objectGUID.oid);
              objectEntryOID=objectEntryAdd(d,&objectGUID,(void*)pp);
              objectEntryOID->privateCreated=ORTE_FALSE;
              pthread_rwlock_wrlock(&d->psEntry.publicationsLock);
              PublicationList_insert(&d->psEntry,objectEntryOID);
              pthread_rwlock_unlock(&d->psEntry.publicationsLock);
              NewPublisher(d,objectEntryOID);
            } else
              FREE(pp);
          } else {
            if ((!PublicationList_find(&d->psEntry,&objectGUID)) &&
	         csChange->alive) {
              pthread_rwlock_wrlock(&d->psEntry.publicationsLock);
              PublicationList_insert(&d->psEntry,objectEntryOID);
              pthread_rwlock_unlock(&d->psEntry.publicationsLock);
              NewPublisher(d,objectEntryOID);
            }
          }
        }
        break;
      case OID_WRITE_SUBS:
        pthread_rwlock_wrlock(&d->readerSubscriptions.lock);        
        cstReader=&d->readerSubscriptions;
        cstRemoteWriter=CSTRemoteWriter_find(cstReader,&writerGUID);
        if (cstRemoteWriter) {
          objectEntryOID=objectEntryFind(d,&objectGUID);
          if (!objectEntryOID) {
            ORTESubsProp *sp=(ORTESubsProp*)MALLOC(sizeof(ORTESubsProp));
            SubsParamsInit(sp);
            parameterUpdateSubscription(csChange,sp);
            if (generateEvent(d,&objectGUID,(void*)sp,ORTE_TRUE) &&
                csChange->alive) {
              debug(46,2) ("new subscriber 0x%x-0x%x-0x%x accepted\n",
                            objectGUID.hid,objectGUID.aid,objectGUID.oid);
              objectEntryOID=objectEntryAdd(d,&objectGUID,(void*)sp);
              objectEntryOID->privateCreated=ORTE_FALSE;
              pthread_rwlock_wrlock(&d->psEntry.subscriptionsLock);
              SubscriptionList_insert(&d->psEntry,objectEntryOID);
              pthread_rwlock_unlock(&d->psEntry.subscriptionsLock);
              NewSubscriber(d,objectEntryOID);
            } else
              FREE(sp);
          } else {
            if ((!SubscriptionList_find(&d->psEntry,&objectGUID)) && 
	         csChange->alive) {
              pthread_rwlock_wrlock(&d->psEntry.subscriptionsLock);
              SubscriptionList_insert(&d->psEntry,objectEntryOID);
              pthread_rwlock_unlock(&d->psEntry.subscriptionsLock);
              NewSubscriber(d,objectEntryOID);
            }
          }
        }
        break;
    }
  }
  if (cstReader && cstRemoteWriter) {
    debug(46,10) ("recv: processing CSChange\n");
    if (SeqNumberCmp(sn,cstRemoteWriter->sn)>0) { //have to be sn>writer_sn
      CSTReaderAddCSChange(cstRemoteWriter,csChange);
      CSTReaderProcCSChanges(d,cstRemoteWriter);
      csChange=NULL;
    }
  }  
  if (csChange) {
    //destroy csChange if any
    parameterDelete(csChange);
    FREE(csChange);
  }
  if (cstReader) {
    pthread_rwlock_unlock(&cstReader->lock);
    return;
  }
} 


/**********************************************************************************/
void 
NewPublisher(ORTEDomain *d,ObjectEntryOID *op) {
  ObjectEntryOID     *o;
  ORTEPublProp       *pp;
  CSTWriter          *cstWriter=NULL;
  CSTReader          *cstReader=NULL;
  PatternNode        *pnode;
  
  if ((d==NULL) || (op==NULL)) return;
  pp=(ORTEPublProp*)op->attributes;
  //***************************************
  //Pattern 
  //try to find if subscription exists
  pthread_rwlock_rdlock(&d->patternEntry.lock);
  pthread_rwlock_rdlock(&d->subscriptions.lock);
  gavl_cust_for_each(CSTReader,
                     &d->subscriptions,cstReader) {
    if (cstReader->createdByPattern) {
      ORTESubsProp       *sp;
      sp=(ORTESubsProp*)cstReader->objectEntryOID->attributes;
      if ((strcmp(sp->topic,pp->topic)==0) &&
          (strcmp(sp->typeName,pp->typeName)==0)) 
        break; //found
    }
  }
  pthread_rwlock_unlock(&d->subscriptions.lock);
  if (!cstReader) { //not exists
    ul_list_for_each(Pattern,&d->patternEntry,pnode) {
      if ((fnmatch(pnode->topic,pp->topic,0)==0) &&
          (fnmatch(pnode->type,pp->typeName,0)==0)) {
        //pattern matched
        // free resources
        pthread_rwlock_unlock(&d->readerPublications.lock);        
        pthread_rwlock_unlock(&d->objectEntry.htimRootLock);
        pthread_rwlock_unlock(&d->objectEntry.objRootLock);    
        cstReader=pnode->subscriptionCallBack(
            pp->topic,
            pp->typeName,
            pnode->param);
        if (cstReader) {
          cstReader->createdByPattern=ORTE_TRUE;
        }
        //allocate resources
        pthread_rwlock_wrlock(&d->objectEntry.objRootLock);    
        pthread_rwlock_wrlock(&d->objectEntry.htimRootLock);
        pthread_rwlock_wrlock(&d->readerPublications.lock);        
      }  
    }
  }
  pthread_rwlock_unlock(&d->patternEntry.lock);
  //Pattern end
  pthread_rwlock_rdlock(&d->psEntry.subscriptionsLock);
  gavl_cust_for_each(SubscriptionList,&d->psEntry,o) {
    ORTESubsProp *sp=(ORTESubsProp*)o->attributes;
    if ((strcmp(pp->topic,sp->topic)==0) &&
        (strcmp(pp->typeName,sp->typeName)==0) &&
        (pp->typeChecksum==sp->typeChecksum)) {
      //add Subscription to Publisher (only if private)
      if (op->privateCreated) {
        pthread_rwlock_rdlock(&d->publications.lock);
        if ((cstWriter=CSTWriter_find(&d->publications,&op->guid))) {
          pthread_rwlock_wrlock(&cstWriter->lock);
          if (!CSTRemoteReader_find(cstWriter,&o->guid)) {
            CSTWriterAddRemoteReader(d,cstWriter,o,o->oid);
            debug(46,2) ("0x%x-0x%x-0x%x accepted 0x%x-0x%x-0x%x\n",
                          op->guid.hid,op->guid.aid,op->guid.oid,
                          o->guid.hid,o->guid.aid,o->guid.oid);
          }
          pthread_rwlock_unlock(&cstWriter->lock);
        }
        pthread_rwlock_unlock(&d->publications.lock);
      }
      //add Publisher to Subscriber (only if private)
      if (o->privateCreated) {
        pthread_rwlock_rdlock(&d->subscriptions.lock);
        if ((cstReader=CSTReader_find(&d->subscriptions,&o->guid))) {
          pthread_rwlock_wrlock(&cstReader->lock);
          if (!CSTRemoteWriter_find(cstReader,&op->guid)) {
            CSTReaderAddRemoteWriter(d,cstReader,op,op->oid);
            debug(46,2) ("0x%x-0x%x-0x%x accepted 0x%x-0x%x-0x%x\n",
                          o->guid.hid,o->guid.aid,o->guid.oid,
                          op->guid.hid,op->guid.aid,op->guid.oid);
          }
          pthread_rwlock_unlock(&cstReader->lock);
        }
        pthread_rwlock_unlock(&d->subscriptions.lock);
      }
    }
  } 
  pthread_rwlock_unlock(&d->psEntry.subscriptionsLock);
}              

/**********************************************************************************/
void 
NewSubscriber(ORTEDomain *d,ObjectEntryOID *os) {
  ObjectEntryOID     *o;
  ORTESubsProp       *sp;
  CSTWriter          *cstWriter;
  CSTReader          *cstReader;
  
  if ((d==NULL) || (os==NULL)) return;
  sp=(ORTESubsProp*)os->attributes;
  pthread_rwlock_rdlock(&d->psEntry.publicationsLock);
  gavl_cust_for_each(PublicationList,&d->psEntry,o) {
    ORTEPublProp *pp=(ORTEPublProp*)o->attributes;
    if ((strcmp(sp->topic,pp->topic)==0) &&
        (strcmp(sp->typeName,pp->typeName)==0) &&
        (sp->typeChecksum==pp->typeChecksum)) {
      //add Publication to Subscription (only if private)
      if (os->privateCreated) {
        pthread_rwlock_rdlock(&d->subscriptions.lock);
        if ((cstReader=CSTReader_find(&d->subscriptions,&os->guid))) {
          pthread_rwlock_wrlock(&cstReader->lock);
          if (!CSTRemoteWriter_find(cstReader,&o->guid)) {
            CSTReaderAddRemoteWriter(d,cstReader,o,o->oid);
            debug(46,2) ("0x%x-0x%x-0x%x accepted 0x%x-0x%x-0x%x\n",
                          os->guid.hid,os->guid.aid,os->guid.oid,
                          o->guid.hid,o->guid.aid,o->guid.oid);
          }
          pthread_rwlock_unlock(&cstReader->lock);
        }
        pthread_rwlock_unlock(&d->subscriptions.lock);
      }
      //add Subscriber to Publisher (only if private)
      if (o->privateCreated) {
        pthread_rwlock_rdlock(&d->publications.lock);
        if ((cstWriter=CSTWriter_find(&d->publications,&o->guid))) {
          pthread_rwlock_wrlock(&cstWriter->lock);
          if (!CSTRemoteReader_find(cstWriter,&os->guid)) {
            CSTWriterAddRemoteReader(d,cstWriter,os,os->oid);
            debug(46,2) ("0x%x-0x%x-0x%x accepted 0x%x-0x%x-0x%x\n",
                          o->guid.hid,o->guid.aid,o->guid.oid,
                          os->guid.hid,os->guid.aid,os->guid.oid);
          }
          pthread_rwlock_unlock(&cstWriter->lock);
        }
        pthread_rwlock_unlock(&d->publications.lock);
      }
    }
  } 
  pthread_rwlock_unlock(&d->psEntry.publicationsLock);
}
