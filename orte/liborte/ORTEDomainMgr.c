/*
 *  $Id: ORTEDomainMgr.c,v 0.0.0.1      2003/09/12
 *
 *  DEBUG:  section 29                  Domain manager
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

/*****************************************************************************/
ORTEDomain *
ORTEDomainMgrCreate(int domain, ORTEDomainProp *prop,
                    ORTEDomainAppEvents *events,Boolean suspended) {
  ORTEDomain        *d;
  ObjectEntryOID    *objectEntryOID;
  AppParams         *appParams;
  CSTWriterParams   cstWriterParams;
  CSTReaderParams   cstReaderParams;
  char              iflocal[MAX_INTERFACES*MAX_STRING_IPADDRESS_LENGTH];
  char              sIPAddress[MAX_STRING_IPADDRESS_LENGTH];
  char              sbuff[128];
  int               i;
  uint16_t          port=0;

  debug(29,10) ("ORTEDomainMgrCreate: start\n");
  //Create domainApplication
  d=MALLOC(sizeof(ORTEDomain));
  if (!d) return NULL;  //no memory
  //initialization local values
  d->domain=domain;
  d->taskRecvMetatraffic.terminate=ORTE_TRUE;
  d->taskSend.terminate=ORTE_TRUE;
  d->taskRecvMetatraffic.sock.port=0;
  d->taskSend.sock.port=0;
  //init structure objectEntry
  ObjectEntryHID_init_root_field(&d->objectEntry);
  pthread_rwlock_init(&d->objectEntry.objRootLock,NULL);
  htimerRoot_init_queue(&d->objectEntry);
  pthread_rwlock_init(&d->objectEntry.htimRootLock,NULL);
  pthread_cond_init(&d->objectEntry.htimSendCond,NULL);
  pthread_mutex_init(&d->objectEntry.htimSendMutex,NULL);
  d->objectEntry.htimSendCondValue=0;
  
  //create domainProp 
  if (prop!=NULL) {
    memcpy(&d->domainProp,prop,sizeof(ORTEDomainProp));
  } else {
    ORTEDomainPropDefaultGet(&d->domainProp);
  }
  
  //print local IP addresses
  iflocal[0]=0;
  if (d->domainProp.IFCount) {
    for(i=0;i<d->domainProp.IFCount;i++) 
      strcat(iflocal,IPAddressToString(d->domainProp.IFProp[i].ipAddress,sIPAddress));
    debug(29,2) ("ORTEDomainMgrCreate: localIPAddres(es) %s\n",iflocal);
  } else{
    debug(29,2) ("ORTEDomainMgrCreate: no active interface card\n");
  }

  //DomainEvents
  if (events!=NULL) {
    memcpy(&d->domainEvents,events,sizeof(ORTEDomainAppEvents));
  } else {
    memset(&d->domainEvents,0,sizeof(ORTEDomainAppEvents));
  }

  //local buffers
  d->mbRecvMetatraffic.cdrStream.buffer=
      (uint8_t*)MALLOC(d->domainProp.recvBuffSize);
  d->mbRecvUserdata.cdrStream.buffer=NULL;
  d->mbSend.cdrStream.buffer=
      (uint8_t*)MALLOC(d->domainProp.sendBuffSize);
  if ((!d->mbRecvMetatraffic.cdrStream.buffer) || 
      (!d->mbSend.cdrStream.buffer)) {    //no memory
    FREE(d->mbRecvMetatraffic.cdrStream.buffer);
    FREE(d->mbSend.cdrStream.buffer);
    FREE(d);
    return NULL;
  }
  d->mbRecvMetatraffic.cdrStream.bufferPtr=d->mbRecvMetatraffic.cdrStream.buffer;
  d->mbRecvMetatraffic.cdrStream.length=0;
  d->mbRecvUserdata.cdrStream.bufferPtr=d->mbRecvUserdata.cdrStream.buffer;
  d->mbRecvUserdata.cdrStream.length=0;
  d->mbSend.cdrStream.bufferPtr=d->mbSend.cdrStream.buffer;
  d->mbSend.cdrStream.length=0;

  //Sockets
  sock_init_udp(&d->taskRecvMetatraffic.sock);
  sock_init_udp(&d->taskSend.sock);
  if (d->domainProp.multicast.enabled) {
    Domain2PortMulticastMetatraffic(d->domain,port);   
  } else {
    Domain2Port(d->domain,port);
  }
  sock_bind(&d->taskRecvMetatraffic.sock,port);    //receiving port
  debug(29,2) ("ORTEDomainMgrCreate: bind on port(Recv): %u\n",
               d->taskRecvMetatraffic.sock.port);
  sock_bind(&d->taskSend.sock,0);       //give me sending port
  debug(29,2) ("ORTEDomainAppCreate: bind on port(Send): %u\n",
               d->taskSend.sock.port);
  if (d->domainProp.multicast.enabled) {
    struct ip_mreq mreq;
    //ttl
    if(sock_setsockopt(&d->taskSend.sock,IP_MULTICAST_TTL, 
        &d->domainProp.multicast.ttl,sizeof(d->domainProp.multicast.ttl))>=0) {
      debug(29,2) ("ORTEDomainAppCreate: ttl set on: %u\n",
           d->domainProp.multicast.ttl);
    } 
    // join multicast group
    mreq.imr_multiaddr.s_addr=htonl(d->domainProp.multicast.ipAddress);
    mreq.imr_interface.s_addr=htonl(INADDR_ANY);
    if(sock_setsockopt(&d->taskRecvUserdata.sock,IP_ADD_MEMBERSHIP,
        (void *) &mreq, sizeof(mreq))>=0) {
      debug(29,2) ("ORTEDomainAppCreate: listening to mgroup %s\n",
           IPAddressToString(d->domainProp.multicast.ipAddress,sIPAddress));
    }
  }
  if ((d->taskRecvMetatraffic.sock.fd<0) || (d->taskSend.sock.fd<0) ||
      (port!=d->taskRecvMetatraffic.sock.port))  {
    printf("Error creating socket(s).\n");
    sock_cleanup(&d->taskRecvMetatraffic.sock);
    sock_cleanup(&d->taskSend.sock);
    FREE(d->mbRecvMetatraffic.cdrStream.buffer);
    FREE(d->mbSend.cdrStream.buffer);
    FREE(d);
    return NULL;
  }

  //Generates local GUID
  if (d->domainProp.IFCount>0) 
    d->guid.hid=d->domainProp.IFProp[0].ipAddress;
  else
    d->guid.hid=StringToIPAddress("127.0.0.1");
  d->guid.aid=(d->taskSend.sock.port<<8)+MANAGER; 
  d->guid.oid=OID_APP;
  debug(29,2) ("ORTEDomainMgrCreate: GUID: %#10.8x,%#10.8x,%#10.8x\n",
               d->guid.hid,d->guid.aid,d->guid.oid); 

  //create HEADER of message for sending task
  RTPSHeaderCreate(d->mbSend.cdrStream.buffer,d->guid.hid,d->guid.aid);
  d->mbSend.cdrStream.bufferPtr=
      d->mbSend.cdrStream.buffer+RTPS_HEADER_LENGTH;
  d->mbSend.cdrStream.length=RTPS_HEADER_LENGTH;    
  d->mbSend.needSend=ORTE_FALSE;
  d->mbSend.containsInfoReply=ORTE_FALSE;  

  //Self object data & fellow managers object data
  appParams=(AppParams*)MALLOC(sizeof(AppParams));
  AppParamsInit(appParams);
  appParams->expirationTime=d->domainProp.baseProp.expirationTime;
  VENDOR_ID_OCERA(appParams->vendorId);
  appParams->hostId=d->guid.hid;
  appParams->appId=d->guid.aid;
  appParams->metatrafficUnicastPort=d->taskRecvMetatraffic.sock.port;
  appParams->userdataUnicastPort=0;  //Manager support only metatraffic
  if (d->domainProp.multicast.enabled) {
    //multicast
    for(i=0;i<d->domainProp.IFCount;i++)
      appParams->metatrafficMulticastIPAddressList[i]=d->domainProp.IFProp[i].ipAddress;
    appParams->metatrafficMulticastIPAddressCount=d->domainProp.IFCount;
  } else {
    //unicast
    if (d->domainProp.IFCount) {
      for(i=0;i<d->domainProp.IFCount;i++)
        appParams->unicastIPAddressList[i]=d->domainProp.IFProp[i].ipAddress;
      appParams->unicastIPAddressCount=d->domainProp.IFCount;
    } else {
      appParams->unicastIPAddressList[0]=StringToIPAddress("127.0.0.1");
      appParams->unicastIPAddressCount=1;
    }
  }
  //managerKeyList
  if (!d->domainProp.keys) {
    appParams->managerKeyList[0]=StringToIPAddress("127.0.0.1");
    for(i=0;i<d->domainProp.IFCount;i++)
      appParams->managerKeyList[i+1]=d->domainProp.IFProp[i].ipAddress;
    appParams->managerKeyCount=d->domainProp.IFCount+1;
  } else {
    appParams->managerKeyCount=i=0;
    while (getStringPart(d->domainProp.keys,':',&i,sbuff)) {
      printf("a");
      ORTESleepMs(100);
      appParams->managerKeyList[appParams->managerKeyCount++]=
          StringToIPAddress(sbuff);
    }
    
  }
  d->appParams=appParams;
  //insert object, doesn't need to be locked
  d->objectEntryOID=objectEntryAdd(d,&d->guid,(void*)appParams);
  d->objectEntryOID->privateCreated=ORTE_TRUE;

  //CST objects
  //  writerApplicationSelf (WAS)
  NTPTIME_ZERO(cstWriterParams.waitWhileDataUnderwayTime);
  cstWriterParams.refreshPeriod=d->domainProp.baseProp.refreshPeriod;
  cstWriterParams.repeatAnnounceTime=d->domainProp.baseProp.repeatAnnounceTime;
  NTPTIME_ZERO(cstWriterParams.delayResponceTime);
  cstWriterParams.HBMaxRetries=d->domainProp.baseProp.HBMaxRetries;
  cstWriterParams.fullAcknowledge=ORTE_FALSE;
  CSTWriterInit(d,&d->writerApplicationSelf,d->objectEntryOID,
      OID_WRITE_APPSELF,&cstWriterParams,NULL);
  //  add to WAS remote writer(s)
  i=0;
  while (getStringPart(d->domainProp.mgrs,':',&i,sbuff)>0) {
    GUID_RTPS guid;
    IPAddress ipAddress=StringToIPAddress(sbuff);
    guid.hid=ipAddress;
    guid.aid=AID_UNKNOWN;
    guid.oid=OID_APP;
    if (!objectEntryFind(d,&guid)) {
      appParams=(AppParams*)MALLOC(sizeof(AppParams));
      AppParamsInit(appParams);
      appParams->hostId=guid.hid;
      appParams->appId=guid.aid;
      appParams->metatrafficUnicastPort=d->appParams->metatrafficUnicastPort;
      appParams->userdataUnicastPort=0;  //Manager support only metatraffic
      appParams->unicastIPAddressList[0]=ipAddress;
      appParams->unicastIPAddressCount=1;
      objectEntryOID=objectEntryAdd(d,&guid,(void*)appParams);
      CSTWriterAddRemoteReader(d,&d->writerApplicationSelf,objectEntryOID,
          OID_READ_MGR);
      debug(29,2) ("ORTEDomainAppCreate: add fellow manager (%s)\n",
                  IPAddressToString(ipAddress,sIPAddress));
    }
  }
  //  readerManagers
  cstReaderParams.delayResponceTimeMin=d->domainProp.baseProp.delayResponceTimeACKMin;
  cstReaderParams.delayResponceTimeMax=d->domainProp.baseProp.delayResponceTimeACKMax;
  cstReaderParams.ACKMaxRetries=d->domainProp.baseProp.ACKMaxRetries;
  cstReaderParams.repeatActiveQueryTime=iNtpTime;  //RM cann't repeatly send ACKf
  cstReaderParams.fullAcknowledge=ORTE_FALSE;      //never will send ACK
  CSTReaderInit(d,&d->readerManagers,d->objectEntryOID,
      OID_READ_MGR,&cstReaderParams,NULL);
  //  readerApplications
  cstReaderParams.delayResponceTimeMin=d->domainProp.baseProp.delayResponceTimeACKMin;
  cstReaderParams.delayResponceTimeMax=d->domainProp.baseProp.delayResponceTimeACKMax;
  cstReaderParams.ACKMaxRetries=d->domainProp.baseProp.ACKMaxRetries;
  cstReaderParams.repeatActiveQueryTime=d->domainProp.baseProp.repeatActiveQueryTime;
  cstReaderParams.fullAcknowledge=ORTE_TRUE;
  CSTReaderInit(d,&d->readerApplications,d->objectEntryOID,
      OID_READ_APP,&cstReaderParams,NULL);
  //  writerApplications
  NTPTIME_ZERO(cstWriterParams.waitWhileDataUnderwayTime);
  cstWriterParams.refreshPeriod=iNtpTime;  //only WAS,WM can refresh csChange(s)
  cstWriterParams.repeatAnnounceTime=d->domainProp.baseProp.repeatAnnounceTime;
  NtpTimeAssembFromMs(cstWriterParams.delayResponceTime,0,20);
  cstWriterParams.HBMaxRetries=d->domainProp.baseProp.HBMaxRetries;
  cstWriterParams.fullAcknowledge=ORTE_FALSE;
  CSTWriterInit(d,&d->writerApplications,d->objectEntryOID,
      OID_WRITE_APP,&cstWriterParams,NULL);
  //  writerManagers
  NTPTIME_ZERO(cstWriterParams.waitWhileDataUnderwayTime);
  cstWriterParams.refreshPeriod=d->domainProp.baseProp.refreshPeriod; 
  cstWriterParams.repeatAnnounceTime=d->domainProp.baseProp.repeatAnnounceTime;
  NtpTimeAssembFromMs(cstWriterParams.delayResponceTime,0,20);
  cstWriterParams.HBMaxRetries=d->domainProp.baseProp.HBMaxRetries;
  cstWriterParams.fullAcknowledge=ORTE_TRUE;
  CSTWriterInit(d,&d->writerManagers,d->objectEntryOID,
      OID_WRITE_MGR,&cstWriterParams,NULL);
  
  //add csChange for WAS
  appSelfParamChanged(d,ORTE_FALSE,ORTE_FALSE,ORTE_FALSE,ORTE_TRUE);
  
  //Start threads
  if (!suspended) {
    ORTEDomainStart(d,ORTE_TRUE,ORTE_FALSE,ORTE_TRUE);
  }

  debug(29,10) ("ORTEDomainMgrCreate: finished\n");
  return d;
}

/*****************************************************************************/
Boolean
ORTEDomainMgrDestroy(ORTEDomain *d) {

  debug(29,10) ("ORTEDomainMgrDestroy: start\n");
  pthread_rwlock_wrlock(&d->objectEntry.objRootLock);
  pthread_rwlock_wrlock(&d->objectEntry.htimRootLock);
  appSelfParamChanged(d,ORTE_TRUE,ORTE_TRUE,ORTE_FALSE,ORTE_FALSE);    
  pthread_rwlock_unlock(&d->objectEntry.htimRootLock);
  pthread_rwlock_unlock(&d->objectEntry.objRootLock);
  //Stoping threads
  if(!d->taskRecvMetatraffic.terminate) {
    d->taskRecvMetatraffic.terminate=ORTE_TRUE;
    ORTEDomainWakeUpReceivingThread(d,
        &d->taskSend.sock,d->taskRecvMetatraffic.sock.port); 
    pthread_join(d->taskRecvMetatraffic.thread,NULL); 
  }
  if (!d->taskSend.terminate) {
    d->taskSend.terminate=ORTE_TRUE;
    ORTEDomainWakeUpSendingThread(&d->objectEntry); 
    pthread_join(d->taskSend.thread,NULL); 
  }
  debug(29,8) ("ORTEDomainMgrDestroy: threads stoped and destroyed\n");

  objectEntryDump(&d->objectEntry);  
    
  //Sockets
  sock_cleanup(&d->taskRecvMetatraffic.sock);
  sock_cleanup(&d->taskSend.sock);

  //Signals
  pthread_cond_destroy(&d->objectEntry.htimSendCond);
  pthread_mutex_destroy(&d->objectEntry.htimSendMutex);
  
  //rwLocks
  pthread_rwlock_destroy(&d->objectEntry.objRootLock);
  pthread_rwlock_destroy(&d->objectEntry.htimRootLock);

  //CSTReaders and CSTWriters
  CSTReaderDelete(d,&d->readerManagers);
  CSTReaderDelete(d,&d->readerApplications);
  CSTWriterDelete(d,&d->writerManagers);
  CSTWriterDelete(d,&d->writerApplications);
  CSTWriterDelete(d,&d->writerApplicationSelf);
  
  //objects in objectsEntry
  objectEntryDeleteAll(d,&d->objectEntry);
  
  FREE(d->mbRecvMetatraffic.cdrStream.buffer);
  FREE(d->mbSend.cdrStream.buffer);
  FREE(d);
  debug(29,10) ("ORTEDomainMgrDestroy: finished\n");
  return ORTE_TRUE;
}
