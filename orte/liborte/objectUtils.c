    /*
 *  $Id: objectUtils.c,v 0.0.0.1         2003/09/10
 *
 *  DEBUG:  section 9                   Functions with objets
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

/*
 * AppParamsInit - Initalization default values of an application
 * @ap: pointer to an application 
 *
 */
void 
AppParamsInit(AppParams *ap) {
  PROTOCOL_VERSION_1_0(ap->protocolVersion);
  VENDOR_ID_UNKNOWN(ap->vendorId);
  NTPTIME_BUILD(ap->expirationTime,180);
  ap->metatrafficUnicastPort=PORT_INVALID;
  ap->userdataUnicastPort=PORT_INVALID;
  ap->unicastIPAddressCount=0;
  ap->metatrafficMulticastIPAddressCount=0;
  ap->managerKeyCount=0;
  SEQUENCE_NUMBER_NONE(ap->vargAppsSequenceNumber);
}

/*
 * PublParamsInit - Initalization default values of a publisher
 * @p: pointer to a publisher
 *
 */
void 
PublParamsInit(ORTEPublProp *pp) {
  pp->expectsAck=ORTE_TRUE;
  NTPTIME_ZERO(pp->persistence);
  pp->reliabilityOffered=0;
  pp->sendQueueSize=1;
  pp->strength=1;
  strcpy(pp->topic,"DefaultTopic");
  strcpy(pp->typeName,"");
  pp->typeChecksum=0;
  //additional params
  NTPTIME_BUILD(pp->maxBlockTime,30);
  pp->criticalQueueLevel=1;
  NtpTimeAssembFromMs(pp->HBNornalRate,1,0);
  NtpTimeAssembFromMs(pp->HBCQLRate,0,50);
  pp->HBMaxRetries=10;
}

/*
 * SubsParamsInit - Initalization default values of a subscription
 * @s: pointer to a subscription
 *
 */
void 
SubsParamsInit(ORTESubsProp *sp) {
  NTPTIME_ZERO(sp->minimumSeparation);
  sp->recvQueueSize=1;
  sp->reliabilityRequested=0;
  strcpy(sp->topic,"DefaultTopic");
  strcpy(sp->typeName,"");
  sp->typeChecksum=0;
//  s->rcvMessageCallBack=NULL;
//  s->deadline=NTPTIME_ZERO;
}

/*
 * UpdateORTEAppInfo - 
 * @appParams: pointer to a source of data
 * @appInfo: pointer to a destination of data
 *
 */
void
updateORTEAppInfo(AppParams *appParams,ORTEAppInfo *appInfo) {
  appInfo->hostId=
      appParams->hostId;
  appInfo->appId=
      appParams->appId;
  appInfo->unicastIPAddressList=
      appParams->unicastIPAddressList;
  appInfo->unicastIPAddressCount=
      appParams->unicastIPAddressCount;
  appInfo->metatrafficMulticastIPAddressList=
      appParams->metatrafficMulticastIPAddressList;
  appInfo->metatrafficMulticastIPAddressCount=
      appParams->metatrafficMulticastIPAddressCount;
  appInfo->metatrafficUnicastPort=
      appParams->metatrafficUnicastPort;
  appInfo->userdataUnicastPort=
      appParams->userdataUnicastPort;
  appInfo->vendorId=
      appParams->vendorId;
  appInfo->protocolVersion=
     appParams->protocolVersion;
}

/*
 * generateEvent - Generate new and delete event up to an application
 * @objectEntryOID: pointer to a OID structure
 * @live: ORTE_TRUE if object is live
 *
 * Return: return ORTE_TRUE if object is acceptable by ORTE (new events)
 */
Boolean
generateEvent(ORTEDomain *d,GUID_RTPS *guid,void *params,Boolean live) {
  ObjectEntryOID  *objectEntryOID;
  GUID_RTPS       guidapp;
  Boolean         result=ORTE_TRUE;
  
  //generate event 
  switch (guid->oid & 0x07) {
    case OID_APPLICATION:
        updateORTEAppInfo((AppParams*)params,&d->appInfo);
        if (((guid->aid & 0x03)==MANAGER) && 
            (d->domainEvents.onMgrNew) && live) 
          result=d->domainEvents.onMgrNew(&d->appInfo,
                                          d->domainEvents.onMgrNewParam);
        if (((guid->aid & 0x03)==MANAGER) && 
            (d->domainEvents.onMgrDelete) && (!live)) 
          d->domainEvents.onMgrDelete(&d->appInfo,
                                      d->domainEvents.onMgrDeleteParam);
        if (((guid->aid & 0x03)==MANAGEDAPPLICATION) && 
            (d->domainEvents.onAppRemoteNew) && live) 
          result=d->domainEvents.onAppRemoteNew(&d->appInfo,
                                          d->domainEvents.onAppRemoteNewParam);
        if (((guid->aid & 0x03)==MANAGEDAPPLICATION) && 
            (d->domainEvents.onAppDelete) && !live) 
          d->domainEvents.onAppDelete(&d->appInfo,
                                      d->domainEvents.onAppDeleteParam);
      break;
    case OID_PUBLICATION:
      guidapp.hid=guid->hid;
      guidapp.aid=guid->aid;
      guidapp.oid=OID_APP;
      objectEntryOID=objectEntryFind(d,&guidapp);
      if (!objectEntryOID) return ORTE_FALSE;
      updateORTEAppInfo((AppParams*)objectEntryOID->attributes,&d->appInfo);   
      d->pubInfo.topic=((ORTEPublProp*)params)->topic;
      d->pubInfo.type=((ORTEPublProp*)params)->typeName;
      d->pubInfo.objectId=guid->oid;
      if ((d->domainEvents.onPubRemoteNew) && live) 
        result=d->domainEvents.onPubRemoteNew(&d->appInfo,&d->pubInfo,
                                        d->domainEvents.onPubRemoteNewParam);
      if ((d->domainEvents.onPubDelete) && !live) 
        d->domainEvents.onPubDelete(&d->appInfo,&d->pubInfo,
                                    d->domainEvents.onPubDeleteParam);
      
      break;
    case OID_SUBSCRIPTION:
      guidapp.hid=guid->hid;
      guidapp.aid=guid->aid;
      guidapp.oid=OID_APP;
      objectEntryOID=objectEntryFind(d,&guidapp);
      if (!objectEntryOID) return ORTE_FALSE;
      updateORTEAppInfo((AppParams*)objectEntryOID->attributes,&d->appInfo);   
      d->subInfo.topic=((ORTESubsProp*)params)->topic;
      d->subInfo.type=((ORTESubsProp*)params)->typeName;
      d->subInfo.objectId=guid->oid;
      if ((d->domainEvents.onSubRemoteNew) && (live))
        result=d->domainEvents.onSubRemoteNew(&d->appInfo,&d->subInfo,
                                        d->domainEvents.onSubRemoteNewParam);
      if ((d->domainEvents.onSubDelete) && (!live))
        d->domainEvents.onSubDelete(&d->appInfo,&d->subInfo,
                                    d->domainEvents.onSubDeleteParam);
      
      break;
  }
  return result;
}

/*
 * GetTypeApp - Test if application is MOM or OAM
 * @d: pointer to a domain
 * @ap 
 * @senderIPAddress:
 *
 * Return: return ORTE_TRUE if application MOM
 */
Boolean
getTypeApp(ORTEDomain *d,AppParams *ap,IPAddress senderIPAddress) {
  Boolean       result=ORTE_FALSE;
  u_int16_t     i,j,k;
  
  //test if the app is MOM
  for (i=0;i<d->appParams->managerKeyCount;i++) {
    for (j=0;j<ap->managerKeyCount;j++) {
      if (ap->managerKeyList[j]!=StringToIPAddress("127.0.0.1")) {
        if (ap->managerKeyList[j]==d->appParams->managerKeyList[i])
          result=ORTE_TRUE;
      } else {
        if (senderIPAddress==StringToIPAddress("127.0.0.1"))
          result=ORTE_TRUE;
        for (k=0;k<d->domainProp.IFCount;k++) {
          if (d->domainProp.IFProp[k].ipAddress==senderIPAddress)
            result=ORTE_TRUE;
        }
      }
    }
  }
  return result;
}

/*
 * appSelfParamChanged - Self parameters changed
 * @d: pointer to an domain
 * @lock: lock CSTWriters at the start of function
 * @unlock: unlock CSTWriters at the end of function
 * @alive:
 *
 */
void 
appSelfParamChanged(ORTEDomain *d,
    Boolean lock,Boolean unlock,Boolean forWM,Boolean alive) {
  CSChange     *csChange;
     
  if (lock) {
    pthread_rwlock_wrlock(&d->writerApplicationSelf.lock);
    if (forWM)
      pthread_rwlock_wrlock(&d->writerManagers.lock);
  }
  csChange=(CSChange*)MALLOC(sizeof(CSChange));
  parameterUpdateCSChange(csChange,d->appParams,ORTE_TRUE);
  csChange->guid=d->guid;
  csChange->alive=alive;
  csChange->cdrStream.buffer=NULL;
  CSTWriterAddCSChange(d,&d->writerApplicationSelf,csChange);
  if (forWM) {
    csChange=(CSChange*)MALLOC(sizeof(CSChange));
    parameterUpdateCSChange(csChange,d->appParams,ORTE_TRUE);
    csChange->guid=d->guid;
    csChange->alive=alive;
    csChange->cdrStream.buffer=NULL;
    CSTWriterAddCSChange(d,&d->writerManagers,csChange);
  }
  if (unlock) {
    pthread_rwlock_unlock(&d->writerApplicationSelf.lock);
    if (forWM)
      pthread_rwlock_unlock(&d->writerManagers.lock);
  }
}
