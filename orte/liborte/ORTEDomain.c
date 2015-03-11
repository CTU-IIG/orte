/*
 *  $Id: ORTEDomain.c,v 0.0.0.1         2003/08/21
 *
 *  DEBUG:  section 30                  Domain functions
 *
 *  -------------------------------------------------------------------
 *                                ORTE
 *                      Open Real-Time Ethernet
 *
 *                      Copyright (C) 2001-2006
 *  Department of Control Engineering FEE CTU Prague, Czech Republic
 *                      http://dce.felk.cvut.cz
 *                      http://www.ocera.org
 *
 *  Author:              Petr Smolik	petr@smoliku.cz
 *  Advisor:             Pavel Pisa
 *  Project Responsible: Zdenek Hanzalek
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
void
ORTEDomainRecvThreadStart(TaskProp *tp)
{
  if (tp->terminate) {
    tp->terminate = ORTE_FALSE;
    pthread_create(&(tp->thread), NULL,
		   (void *)&ORTEAppRecvThread, (void *)tp);
  }
}

/*****************************************************************************/
void
ORTEDomainSendThreadStart(TaskProp *tp)
{
  if (tp->terminate) {
    tp->terminate = ORTE_FALSE;
    pthread_create(&(tp->thread), NULL,
		   (void *)&ORTEAppSendThread, (void *)tp);
  }
}

/*****************************************************************************/
void
ORTEDomainRecvThreadStop(TaskProp *tp)
{
  ORTEDomain *d = tp->d;

  if (!tp->terminate) {
    tp->terminate = ORTE_TRUE;
    ORTEDomainWakeUpReceivingThread(d,
				    &d->taskSend.sock, tp->sock.port);
    pthread_join(tp->thread, NULL);
  }
}

/*****************************************************************************/
void
ORTEDomainSendThreadStop(TaskProp *tp)
{
  ORTEDomain *d = tp->d;

  if (!tp->terminate) {
    tp->terminate = ORTE_TRUE;
    ORTEDomainWakeUpSendingThread(&d->objectEntry);
    pthread_join(tp->thread, NULL);
  }
}

/*****************************************************************************/
void
ORTEDomainStart(ORTEDomain *d,
		Boolean recvUnicastMetatrafficThread,
		Boolean recvMulticastMetatrafficThread,
		Boolean recvUnicastUserdataThread,
		Boolean recvMulticastUserdataThread,
		Boolean sendThread)
{

  if (!d)
    return;

  if (recvUnicastMetatrafficThread)
    ORTEDomainRecvThreadStart(&d->taskRecvUnicastMetatraffic);

  if (recvMulticastMetatrafficThread)
    ORTEDomainRecvThreadStart(&d->taskRecvMulticastMetatraffic);

  if (recvUnicastUserdataThread)
    ORTEDomainRecvThreadStart(&d->taskRecvUnicastUserdata);

  if (recvMulticastUserdataThread)
    ORTEDomainRecvThreadStart(&d->taskRecvMulticastUserdata);

  if (sendThread)
    ORTEDomainSendThreadStart(&d->taskSend);
}

/*****************************************************************************/
Boolean
ORTEDomainPropDefaultGet(ORTEDomainProp *prop)
{
  sock_t        sock;

  memset(prop, 0, sizeof(*prop));

  prop->multicast.enabled = ORTE_FALSE;
  prop->multicast.ttl = 1;
  prop->multicast.loopBackEnabled = ORTE_TRUE;

  //IFProp
  sock_init_udp(&sock);
  if (sock_bind(&sock, 0, INADDR_ANY) == -1) {
    return ORTE_FALSE;
  }
  sock_get_local_interfaces(&sock, prop->IFProp, (char *)&prop->IFCount);
  sock_cleanup(&sock);

  prop->mgrs = NULL; //only from localhost
  prop->appLocalManager = StringToIPAddress("127.0.0.1");
  prop->listen = INADDR_ANY;
  prop->keys = NULL; //are assign be orte
  sprintf(prop->version, ORTE_PACKAGE_STRING \
	  ", compiled: " \
	  __DATE__ \
	  " " \
	  __TIME__);

  prop->recvBuffSize = 0x4000;
  prop->sendBuffSize = 0x4000;
  prop->wireProp.metaBytesPerPacket = 1500;
  prop->wireProp.metaBytesPerFastPacket = 1000; //not used
  prop->wireProp.metabitsPerACKBitmap = 32;     //not used
  prop->wireProp.userBytesPerPacket = 0x3000;

  //domainBaseProp
  prop->baseProp.registrationMgrRetries = 0;
  NTPTIME_BUILD(prop->baseProp.registrationMgrPeriod, 0); //0s
  prop->baseProp.registrationAppRetries = 3;
  NtpTimeAssembFromMs(prop->baseProp.registrationAppPeriod, 0, 500); //500ms
  NTPTIME_BUILD(prop->baseProp.expirationTime, 180);  //180s
  NTPTIME_BUILD(prop->baseProp.refreshPeriod, 72);    //72s - refresh self parameters
  NTPTIME_BUILD(prop->baseProp.purgeTime, 60);        //60s - purge time of parameters
  NTPTIME_BUILD(prop->baseProp.repeatAnnounceTime, 72); //72s - announcement by HB
  NTPTIME_BUILD(prop->baseProp.repeatActiveQueryTime, 72); //72s - announcement by ACK
  NtpTimeAssembFromMs(prop->baseProp.delayResponceTimeACKMin, 0, 10); //10ms - delay before send ACK
  NtpTimeAssembFromMs(prop->baseProp.delayResponceTimeACKMax, 1, 0); //1s
  NtpTimeAssembFromMs(prop->baseProp.maxBlockTime, 20, 0); //20s
  prop->baseProp.ACKMaxRetries = 10;
  prop->baseProp.HBMaxRetries = 10;

  PublParamsInit(&prop->publPropDefault);
  SubsParamsInit(&prop->subsPropDefault);

  return ORTE_TRUE;
}

/*****************************************************************************/
Boolean
ORTEDomainInitEvents(ORTEDomainAppEvents *events)
{
  memset(events, 0, sizeof(ORTEDomainAppEvents));
  return ORTE_TRUE;
}


/*****************************************************************************/
static void
generateLocalGUID(ORTEDomain *d, Boolean manager)
{
  if (d->domainProp.IFCount > 0)
    d->guid.hid = d->domainProp.IFProp[0].ipAddress;
  else
    d->guid.hid = StringToIPAddress("127.0.0.1");
  if (manager) {
    d->guid.aid = (d->taskSend.sock.port<<8)+MANAGER;
  } else {
    d->guid.aid = (d->taskSend.sock.port<<8)+MANAGEDAPPLICATION;
  }
  d->guid.oid = OID_APP;
  debug(30, 2) ("ORTEDomainCreate: GUID: %#10.8x,%#10.8x,%#10.8x\n",
		GUID_PRINTF(d->guid));
}

static int
initTaskProp(TaskProp *tp, ORTEDomain *d, int buffSize)
{
  tp->d = d;
  tp->terminate = ORTE_TRUE;
  tp->sock.port = 0;
  CDR_codec_init_static(&tp->mb.cdrCodec);
  if (buffSize) {
    CDR_buffer_init(&tp->mb.cdrCodec, buffSize);
    if (!tp->mb.cdrCodec.buffer)
      return -1;
  }
  return sock_init_udp(&tp->sock);
}

static int
objectEntryInit(ObjectEntry *oe)
{
  pthread_condattr_t attr;

  pthread_condattr_init(&attr);
#if defined HAVE_PTHREAD_CONDATTR_SETCLOCK && HAVE_DECL_CLOCK_MONOTONIC
  if (pthread_condattr_setclock(&attr, CLOCK_MONOTONIC) != 0)
    return -1;
#endif
  ObjectEntryHID_init_root_field(oe);
  pthread_rwlock_init(&oe->objRootLock, NULL);
  htimerRoot_init_queue(oe);
  pthread_rwlock_init(&oe->htimRootLock, NULL);
  pthread_cond_init(&oe->htimSendCond, &attr);
  pthread_mutex_init(&oe->htimSendMutex, NULL);
  oe->htimSendCondValue = 0;
  oe->htimNeedWakeUp = ORTE_TRUE;
  return 0;
}

static void
publicationsInit(CSTPublications *p)
{
  p->counter = 0;
  CSTWriter_init_root_field(p);
  pthread_rwlock_init(&p->lock, NULL);
}

static void
subscriptionsInit(CSTSubscriptions *s)
{
  s->counter = 0;
  CSTReader_init_root_field(s);
  pthread_rwlock_init(&s->lock, NULL);
}

static void
psEntryInit(PSEntry *e)
{
  PublicationList_init_root_field(e);
  pthread_rwlock_init(&e->publicationsLock, NULL);
  SubscriptionList_init_root_field(e);
  pthread_rwlock_init(&e->subscriptionsLock, NULL);
}

static void
patternEntryInit(ORTEDomain *d)
{
  pthread_rwlock_init(&d->patternEntry.lock, NULL);
  Pattern_init_head(&d->patternEntry);
  ORTEPatternRegister(d, ORTEPatternCheckDefault, ORTEPatternMatchDefault, NULL);
}

static void
printLocalAddresses(ORTEDomain *d)
{
  char iflocal[MAX_INTERFACES*MAX_STRING_IPADDRESS_LENGTH];
  char sIPAddress[MAX_STRING_IPADDRESS_LENGTH];
  int i;

  iflocal[0] = 0;
  if (d->domainProp.IFCount) {
    for (i = 0; i < d->domainProp.IFCount; i++)
      strcat(iflocal, IPAddressToString(d->domainProp.IFProp[i].ipAddress, sIPAddress));
    debug(30, 2) ("ORTEDomainCreate: localIPAddres(es) %s\n", iflocal);
  } else {
    debug(30, 2) ("ORTEDomainCreate: no active interface card\n");
  }
}

static void
typeEntryInit(TypeEntry *e)
{
  ORTEType_init_root_field(e);
  pthread_rwlock_init(&e->lock, NULL);
}

static int32_t
bindUnicastMetatrafficSock(ORTEDomain *d, Boolean manager)
{
  uint16_t          port = 0;

  Domain2Port(d->domain, port);
  if (manager) {
    if (d->domainProp.multicast.enabled) {
      char sIPAddress[MAX_STRING_IPADDRESS_LENGTH];
      struct ip_mreq mreq;
      int reuse = 1, loop = 0;

      //reuseaddr
      sock_setsockopt(&d->taskRecvUnicastMetatraffic.sock, SOL_SOCKET,
		      SO_REUSEADDR, (const char *)&reuse, sizeof(reuse));
      debug(30, 2) ("ORTEDomainCreate: set value SO_REUSEADDR: %u\n",
		    reuse);

      //multicast loop
      sock_setsockopt(&d->taskRecvUnicastMetatraffic.sock, IPPROTO_IP,
		      IP_MULTICAST_LOOP, (const char *)&loop,
		      sizeof(loop));
      debug(30, 2) ("ORTEDomainCreate: set value IP_MULTICAST_LOOP: %u\n",
		    loop);

      //join to multicast group
      memset(&mreq, 0, sizeof(mreq));
      mreq.imr_multiaddr.s_addr = htonl(d->domainProp.multicast.ipAddress);
      mreq.imr_interface.s_addr = htonl(INADDR_ANY);
      if (sock_setsockopt(&d->taskRecvUnicastMetatraffic.sock, IPPROTO_IP,
			  IP_ADD_MEMBERSHIP, (const char *)&mreq, sizeof(mreq)) >= 0) {
	debug(30, 2) ("ORTEDomainCreate: joint to mgroup %s\n",
		      IPAddressToString(d->domainProp.multicast.ipAddress, sIPAddress));
      } else
	return -1;
    }
    if (sock_bind(&d->taskRecvUnicastMetatraffic.sock, port, d->domainProp.listen) == -1) {
      return -1;
    }
  } else {
    /* give me receiving port (metatraffic) */
    if (sock_bind(&d->taskRecvUnicastMetatraffic.sock, 0, d->domainProp.listen) == -1) {
      return -1;
    }
  }
  debug(30, 2) ("ORTEDomainCreate: bind on port(RecvUnicastMetatraffic): %u\n",
		d->taskRecvUnicastMetatraffic.sock.port);

  return port;
}

static int
bindMulticastMetatrafficSock(ORTEDomain *d)
{
  char sIPAddress[MAX_STRING_IPADDRESS_LENGTH];
  struct ip_mreq mreq;
  Port mport;
  int reuse = 1;

  //reuseaddr
  sock_setsockopt(&d->taskRecvMulticastMetatraffic.sock, SOL_SOCKET,
		  SO_REUSEADDR, (const char *)&reuse, sizeof(reuse));
  debug(30, 2) ("ORTEDomainCreate: set value SO_REUSEADDR: %u\n",
		reuse);

  //multicast loop
  sock_setsockopt(&d->taskRecvMulticastMetatraffic.sock, IPPROTO_IP,
		  IP_MULTICAST_LOOP, (const char *)&d->domainProp.multicast.loopBackEnabled,
		  sizeof(d->domainProp.multicast.loopBackEnabled));
  debug(30, 2) ("ORTEDomainCreate: set value IP_MULTICAST_LOOP: %u\n",
		d->domainProp.multicast.loopBackEnabled);

  //joint to multicast group
  mreq.imr_multiaddr.s_addr = htonl(d->domainProp.multicast.ipAddress);
  mreq.imr_interface.s_addr = htonl(INADDR_ANY);
  if (sock_setsockopt(&d->taskRecvMulticastMetatraffic.sock, IPPROTO_IP,
		      IP_ADD_MEMBERSHIP, (const char *)&mreq, sizeof(mreq)) >= 0) {
    debug(30, 2) ("ORTEDomainCreate: joint to mgroup %s\n",
		  IPAddressToString(d->domainProp.multicast.ipAddress, sIPAddress));
  }

  /* receiving multicast port (metatraffic) */
  Domain2PortMulticastMetatraffic(d->domain, mport);
  if (sock_bind(&d->taskRecvMulticastMetatraffic.sock, (uint16_t)mport, d->domainProp.listen) == -1) {
    return -1;
  }
  debug(30, 2) ("ORTEDomainCreate: bind on port(RecvMulticastMetatraffic): %u\n",
		d->taskRecvMulticastMetatraffic.sock.port);
  return 0;
}

static int
bindUserDataSockets(ORTEDomain *d)
{
  if (sock_bind(&d->taskRecvUnicastUserdata.sock, 0, d->domainProp.listen) == -1) {
    return -1;
  }
  debug(30, 2) ("ORTEDomainCreate: bind on port(RecvUnicatUserdata): %u\n",
		d->taskRecvUnicastUserdata.sock.port);

  if (d->domainProp.multicast.enabled) {
    Port mport;
    int reuse = 1;

    //reuseaddr
    sock_setsockopt(&d->taskRecvMulticastUserdata.sock, SOL_SOCKET,
		    SO_REUSEADDR, (const char *)&reuse, sizeof(reuse));
    debug(30, 2) ("ORTEDomainCreate: set value SO_REUSEADDR: %u\n",
		  reuse);

    //multicast loop
    sock_setsockopt(&d->taskRecvMulticastUserdata.sock, IPPROTO_IP,
		    IP_MULTICAST_LOOP, (const char *)&d->domainProp.multicast.loopBackEnabled,
		    sizeof(d->domainProp.multicast.loopBackEnabled));
    debug(30, 2) ("ORTEDomainCreate: set value IP_MULTICAST_LOOP: %u\n",
		  d->domainProp.multicast.loopBackEnabled);

    /* receiving multicast port (userdata) */
    Domain2PortMulticastUserdata(d->domain, mport);
    if (sock_bind(&d->taskRecvMulticastUserdata.sock, (uint16_t)mport, d->domainProp.listen) == -1) {
      return -1;
    }
    debug(30, 2) ("ORTEDomainCreate: bind on port(RecvMulticastUserdata): %u\n",
		  d->taskRecvMulticastUserdata.sock.port);
  }
  return 0;
}

static int
bindSendSock(ORTEDomain *d)
{
  if (sock_bind(&d->taskSend.sock, 0, d->domainProp.listen) == -1) {
    return -1;
  }
  debug(30, 2) ("ORTEDomainCreate: bind on port(Send): %u\n",
		d->taskSend.sock.port);
  if (d->domainProp.multicast.enabled) {
    //ttl
    if (sock_setsockopt(&d->taskSend.sock, IPPROTO_IP, IP_MULTICAST_TTL, (const char *)&d->domainProp.multicast.ttl, sizeof(d->domainProp.multicast.ttl)) >= 0) {
      debug(30, 2) ("ORTEDomainCreate: ttl set on: %u\n",
		    d->domainProp.multicast.ttl);
    }
  }
  return 0;
}

ORTEDomain *
ORTEDomainCreate(int domain, ORTEDomainProp *prop,
		 ORTEDomainAppEvents *events, Boolean manager)
{
  ORTEDomain        *d;
  ObjectEntryOID    *objectEntryOID;
  AppParams         *appParams;
  CSTWriterParams   cstWriterParams;
  CSTReaderParams   cstReaderParams;
  char              sbuff[128];
  int               i;
  int               errno_save = 0;

  debug(30, 2)  ("ORTEDomainCreate: %s compiled: %s,%s\n",
		 ORTE_PACKAGE_STRING, __DATE__, __TIME__);

  debug(30, 10) ("ORTEDomainCreate: start\n");
  //Create domainApplication
  d = MALLOC(sizeof(ORTEDomain));
  if (!d)
    return NULL;        //no memory
  //initialization local values
  d->domain = domain;

  //create domainProp
  if (prop != NULL) {
    memcpy(&d->domainProp, prop, sizeof(ORTEDomainProp));
  } else {
    if (!ORTEDomainPropDefaultGet(&d->domainProp)) {
      goto err_domainProp;
    }
  }
  if (!d->domainProp.IFCount && d->domainProp.multicast.enabled) {
    debug(30, 0) ("ORTEDomainCreate: for multicast have to be active an interface\n");
    goto err_domainProp;
  }
  ORTEDomainProp *dp = &d->domainProp;

  if (-1 == initTaskProp(&d->taskRecvUnicastMetatraffic,   d, dp->recvBuffSize) ||
      -1 == initTaskProp(&d->taskRecvUnicastUserdata,      d, !manager ? dp->recvBuffSize : 0) ||
      -1 == initTaskProp(&d->taskRecvMulticastMetatraffic, d, !manager && dp->multicast.enabled ? dp->recvBuffSize : 0) ||
      -1 == initTaskProp(&d->taskRecvMulticastUserdata,    d, !manager && dp->multicast.enabled ? dp->recvBuffSize : 0) ||
      -1 == initTaskProp(&d->taskSend, d, dp->sendBuffSize))
    goto err_domainProp;

  d->taskSend.mb.cdrCodec.wptr_max = dp->wireProp.metaBytesPerPacket;
  assert(d->taskSend.mb.cdrCodec.wptr_max <= d->taskSend.mb.cdrCodec.buf_len);
  d->taskSend.mb.cdrCodec.data_endian = FLAG_ENDIANNESS;

  if (objectEntryInit(&d->objectEntry) == -1)
    goto err_sock;

  publicationsInit(&d->publications);
  subscriptionsInit(&d->subscriptions);
  psEntryInit(&d->psEntry);

  patternEntryInit(d);

  printLocalAddresses(d);

  //DomainEvents
  if (events != NULL) {
    memcpy(&d->domainEvents, events, sizeof(ORTEDomainAppEvents));
  } else {
    memset(&d->domainEvents, 0, sizeof(ORTEDomainAppEvents));
  }

  typeEntryInit(&d->typeEntry);

  // Bind sockets
  int32_t ret = bindUnicastMetatrafficSock(d, manager);
  if (ret == -1)
    goto err_sock;
  uint16_t port = ret & 0xffff;

  if (!manager) {
    if (d->domainProp.multicast.enabled) {
      if (bindMulticastMetatrafficSock(d) == -1)
	goto err_sock;
    }

    if (bindUserDataSockets(d) == -1)
      goto err_sock;
  }

  if (bindSendSock(d) == -1)
    goto err_sock;

  generateLocalGUID(d, manager);

  //create HEADER of message for sending task
  RTPSHeaderCreate(&d->taskSend.mb.cdrCodec, d->guid.hid, d->guid.aid);
  d->taskSend.mb.needSend = ORTE_FALSE;
  d->taskSend.mb.containsInfoReply = ORTE_FALSE;
  d->taskSend.mb.cdrCodecDirect = NULL;

  //Self object data & fellow managers object data
  appParams = (AppParams *)MALLOC(sizeof(AppParams));
  if (!appParams) {
    goto err_sock;
  }
  AppParamsInit(appParams);
  appParams->expirationTime = d->domainProp.baseProp.expirationTime;
  VENDOR_ID_OCERA(appParams->vendorId);
  appParams->hostId = d->guid.hid;
  appParams->appId = d->guid.aid;
  appParams->metatrafficUnicastPort = d->taskRecvUnicastMetatraffic.sock.port;
  appParams->userdataUnicastPort = d->taskRecvUnicastUserdata.sock.port;
  //fill unicast/multicast ip addresses
  if (d->domainProp.IFCount) {
    for (i = 0; i < d->domainProp.IFCount; i++)
      appParams->unicastIPAddressList[i] = d->domainProp.IFProp[i].ipAddress;
    appParams->unicastIPAddressCount = d->domainProp.IFCount;
  }
  if (d->domainProp.multicast.enabled &&
      IN_MULTICAST(d->domainProp.multicast.ipAddress)) {
    appParams->metatrafficMulticastIPAddressList[appParams->metatrafficMulticastIPAddressCount] =
      d->domainProp.multicast.ipAddress;
    appParams->metatrafficMulticastIPAddressCount++;
  } else {
    if (!d->domainProp.IFCount) {
      appParams->unicastIPAddressList[appParams->unicastIPAddressCount] =
	StringToIPAddress("127.0.0.1");
      appParams->unicastIPAddressCount++;
    }
  }
  //KeyList
  if (!d->domainProp.keys) {
    appParams->managerKeyList[0] = StringToIPAddress("127.0.0.1");
    for (i = 0; i < d->domainProp.IFCount; i++)
      appParams->managerKeyList[i+1] = d->domainProp.IFProp[i].ipAddress;
    if (d->domainProp.multicast.enabled &&
	IN_MULTICAST(d->domainProp.multicast.ipAddress)) {
      appParams->managerKeyList[i+1] = d->domainProp.multicast.ipAddress;
      i++;
    }
    appParams->managerKeyCount = i+1;
  } else {
    appParams->managerKeyCount = i = 0;
    while (getStringPart(d->domainProp.keys, ':', &i, sbuff))
      appParams->managerKeyList[appParams->managerKeyCount++] =
	StringToIPAddress(sbuff);
  }
  d->appParams = appParams;
  //insert object, doesn't need to be locked
  d->objectEntryOID = objectEntryAdd(d, &d->guid, (void *)appParams);
  d->objectEntryOID->privateCreated = ORTE_TRUE;


  /************************************************************************/
  //CST objects
  //  writerApplicationSelf (WAS)
  NTPTIME_ZERO(cstWriterParams.waitWhileDataUnderwayTime);
  cstWriterParams.refreshPeriod = d->domainProp.baseProp.refreshPeriod;
  cstWriterParams.repeatAnnounceTime = d->domainProp.baseProp.repeatAnnounceTime;
  NTPTIME_ZERO(cstWriterParams.delayResponceTime);
  cstWriterParams.HBMaxRetries = d->domainProp.baseProp.HBMaxRetries;
  if (manager) {
    cstWriterParams.registrationRetries = d->domainProp.baseProp.registrationMgrRetries;
    cstWriterParams.registrationPeriod = d->domainProp.baseProp.registrationMgrPeriod;
    cstWriterParams.fullAcknowledge = ORTE_FALSE;
  } else {
    cstWriterParams.registrationRetries = d->domainProp.baseProp.registrationAppRetries;
    cstWriterParams.registrationPeriod = d->domainProp.baseProp.registrationAppPeriod;
    cstWriterParams.fullAcknowledge = ORTE_TRUE;
  }
  CSTWriterInit(d, &d->writerApplicationSelf, d->objectEntryOID,
		OID_WRITE_APPSELF, &cstWriterParams, NULL);
  if (manager) {
    i = 0;
    while (getStringPart(d->domainProp.mgrs, ':', &i, sbuff) > 0) {
      GUID_RTPS guid;
      IPAddress ipAddress = StringToIPAddress(sbuff);
      guid.hid = ipAddress;
      guid.aid = AID_UNKNOWN;
      guid.oid = OID_APP;
      if (!objectEntryFind(d, &guid)) {
	char sIPAddress[MAX_STRING_IPADDRESS_LENGTH];
	appParams = (AppParams *)MALLOC(sizeof(AppParams));
	AppParamsInit(appParams);
	appParams->hostId = guid.hid;
	appParams->appId = guid.aid;
	appParams->metatrafficUnicastPort = d->appParams->metatrafficUnicastPort;
	objectEntryOID = objectEntryAdd(d, &guid, (void *)appParams);
	if (d->domainProp.multicast.enabled && IN_MULTICAST(ipAddress)) {
	  appParams->metatrafficMulticastIPAddressList[0] = ipAddress;
	  appParams->metatrafficMulticastIPAddressCount = 1;
	  objectEntryOID->multicastPort = port;
	} else {
	  appParams->unicastIPAddressList[0] = ipAddress;
	  appParams->unicastIPAddressCount = 1;
	  objectEntryOID->multicastPort = 0;
	}
	appParams->userdataUnicastPort = 0;  //Manager support only metatraffic
	CSTWriterAddRemoteReader(d,
				 &d->writerApplicationSelf,
				 objectEntryOID,
				 OID_READ_MGR,
				 objectEntryOID);
	debug(29, 2) ("ORTEDomainCreate: add fellow manager (%s)\n",
		      IPAddressToString(ipAddress, sIPAddress));
      }
    }
  } else {
    //  add to WAS remote writer(s)
    if (d->domainProp.appLocalManager) {
      GUID_RTPS guid;
      guid.hid = d->domainProp.appLocalManager;
      guid.aid = AID_UNKNOWN;
      guid.oid = OID_APP;
      if (!objectEntryFind(d, &guid)) {
	char sIPAddress[MAX_STRING_IPADDRESS_LENGTH];
	appParams = (AppParams *)MALLOC(sizeof(AppParams));
	AppParamsInit(appParams);
	appParams->hostId = guid.hid;
	appParams->appId = guid.aid;
	appParams->metatrafficUnicastPort = port;
	appParams->userdataUnicastPort = 0;  //Manager support only metatraffic
	appParams->unicastIPAddressList[0] = d->domainProp.appLocalManager;
	appParams->unicastIPAddressCount = 1;
	objectEntryOID = objectEntryAdd(d, &guid, (void *)appParams);
	CSTWriterAddRemoteReader(d,
				 &d->writerApplicationSelf,
				 objectEntryOID,
				 OID_READ_MGR,
				 objectEntryOID);
	debug(30, 2) ("ORTEDomainCreate: add manager (%s)\n",
		      IPAddressToString(d->domainProp.appLocalManager, sIPAddress));
      }
    }
  }

  //  readerManagers
  cstReaderParams.delayResponceTimeMin = d->domainProp.baseProp.delayResponceTimeACKMin;
  cstReaderParams.delayResponceTimeMax = d->domainProp.baseProp.delayResponceTimeACKMax;
  cstReaderParams.ACKMaxRetries = d->domainProp.baseProp.ACKMaxRetries;
  if (manager) {
    cstReaderParams.ACKMaxRetries = d->domainProp.baseProp.ACKMaxRetries;
    cstReaderParams.repeatActiveQueryTime = iNtpTime;  //RM cann't repeatly send ACKf
  } else {
    cstReaderParams.repeatActiveQueryTime = d->domainProp.baseProp.repeatActiveQueryTime;
    cstReaderParams.fullAcknowledge = ORTE_TRUE;
  }
  CSTReaderInit(d, &d->readerManagers, d->objectEntryOID,
		OID_READ_MGR, &cstReaderParams, NULL);

  //  readerApplications
  cstReaderParams.delayResponceTimeMin = d->domainProp.baseProp.delayResponceTimeACKMin;
  cstReaderParams.delayResponceTimeMax = d->domainProp.baseProp.delayResponceTimeACKMax;
  cstReaderParams.ACKMaxRetries = d->domainProp.baseProp.ACKMaxRetries;
  cstReaderParams.repeatActiveQueryTime = d->domainProp.baseProp.repeatActiveQueryTime;
  cstReaderParams.fullAcknowledge = ORTE_TRUE;
  CSTReaderInit(d, &d->readerApplications, d->objectEntryOID,
		OID_READ_APP, &cstReaderParams, NULL);

  if (manager) {
    //  writerApplications
    cstWriterParams.registrationRetries = 0;
    NTPTIME_ZERO(cstWriterParams.registrationPeriod);
    NTPTIME_ZERO(cstWriterParams.waitWhileDataUnderwayTime);
    cstWriterParams.refreshPeriod = iNtpTime;  //only WAS,WM can refresh csChange(s)
    cstWriterParams.repeatAnnounceTime = d->domainProp.baseProp.repeatAnnounceTime;
    NtpTimeAssembFromMs(cstWriterParams.delayResponceTime, 0, 20);
    cstWriterParams.HBMaxRetries = d->domainProp.baseProp.HBMaxRetries;
    cstWriterParams.fullAcknowledge = ORTE_FALSE;
    CSTWriterInit(d, &d->writerApplications, d->objectEntryOID,
		  OID_WRITE_APP, &cstWriterParams, NULL);

    //  writerManagers
    cstWriterParams.registrationRetries = 0;
    NTPTIME_ZERO(cstWriterParams.registrationPeriod);
    NTPTIME_ZERO(cstWriterParams.waitWhileDataUnderwayTime);
    cstWriterParams.refreshPeriod = d->domainProp.baseProp.refreshPeriod;
    cstWriterParams.repeatAnnounceTime = d->domainProp.baseProp.repeatAnnounceTime;
    NtpTimeAssembFromMs(cstWriterParams.delayResponceTime, 0, 20);
    cstWriterParams.HBMaxRetries = d->domainProp.baseProp.HBMaxRetries;
    cstWriterParams.fullAcknowledge = ORTE_TRUE;
    CSTWriterInit(d, &d->writerManagers, d->objectEntryOID,
		  OID_WRITE_MGR, &cstWriterParams, NULL);
  }

  if (!manager) {
    //  writerPublications
    cstWriterParams.registrationRetries = 0;
    NTPTIME_ZERO(cstWriterParams.registrationPeriod);
    NTPTIME_ZERO(cstWriterParams.waitWhileDataUnderwayTime);
    cstWriterParams.refreshPeriod = d->domainProp.baseProp.refreshPeriod;
    cstWriterParams.repeatAnnounceTime = d->domainProp.baseProp.repeatAnnounceTime;
    NtpTimeAssembFromMs(cstWriterParams.delayResponceTime, 0, 20);
    cstWriterParams.HBMaxRetries = d->domainProp.baseProp.HBMaxRetries;
    cstWriterParams.fullAcknowledge = ORTE_TRUE;
    CSTWriterInit(d, &d->writerPublications, d->objectEntryOID,
		  OID_WRITE_PUBL, &cstWriterParams, NULL);
    //  writerSubscriptions
    cstWriterParams.registrationRetries = 0;
    NTPTIME_ZERO(cstWriterParams.registrationPeriod);
    NTPTIME_ZERO(cstWriterParams.waitWhileDataUnderwayTime);
    cstWriterParams.refreshPeriod = d->domainProp.baseProp.refreshPeriod;
    cstWriterParams.repeatAnnounceTime = d->domainProp.baseProp.repeatAnnounceTime;
    NtpTimeAssembFromMs(cstWriterParams.delayResponceTime, 0, 20);
    cstWriterParams.HBMaxRetries = d->domainProp.baseProp.HBMaxRetries;
    cstWriterParams.fullAcknowledge = ORTE_TRUE;
    CSTWriterInit(d, &d->writerSubscriptions, d->objectEntryOID,
		  OID_WRITE_SUBS, &cstWriterParams, NULL);
    //  readerPublications
    cstReaderParams.delayResponceTimeMin = d->domainProp.baseProp.delayResponceTimeACKMin;
    cstReaderParams.delayResponceTimeMax = d->domainProp.baseProp.delayResponceTimeACKMax;
    cstReaderParams.ACKMaxRetries = d->domainProp.baseProp.ACKMaxRetries;
    cstReaderParams.repeatActiveQueryTime = d->domainProp.baseProp.repeatActiveQueryTime;
    cstReaderParams.fullAcknowledge = ORTE_TRUE;
    CSTReaderInit(d, &d->readerPublications, d->objectEntryOID,
		  OID_READ_PUBL, &cstReaderParams, NULL);
    //  readerSubscriptions
    cstReaderParams.delayResponceTimeMin = d->domainProp.baseProp.delayResponceTimeACKMin;
    cstReaderParams.delayResponceTimeMax = d->domainProp.baseProp.delayResponceTimeACKMax;
    cstReaderParams.ACKMaxRetries = d->domainProp.baseProp.ACKMaxRetries;
    cstReaderParams.repeatActiveQueryTime = d->domainProp.baseProp.repeatActiveQueryTime;
    cstReaderParams.fullAcknowledge = ORTE_TRUE;
    CSTReaderInit(d, &d->readerSubscriptions, d->objectEntryOID,
		  OID_READ_SUBS, &cstReaderParams, NULL);
  }

  //add csChange for WAS
  appSelfParamChanged(d, ORTE_FALSE, ORTE_FALSE, ORTE_FALSE, ORTE_TRUE);

  debug(30, 10) ("ORTEDomainCreate: finished\n");
  return d;

//err:
  if (!errno_save)
    errno_save = errno;
  /* TODO */
  FREE(appParams);
err_sock:
  if (!errno_save)
    errno_save = errno;
  sock_cleanup(&d->taskRecvUnicastMetatraffic.sock);
  sock_cleanup(&d->taskRecvMulticastMetatraffic.sock);
  sock_cleanup(&d->taskRecvUnicastUserdata.sock);
  sock_cleanup(&d->taskRecvMulticastUserdata.sock);
  sock_cleanup(&d->taskSend.sock);
  pthread_rwlock_destroy(&d->typeEntry.lock);
  if (d->domainProp.multicast.enabled) {
    CDR_codec_release_buffer(&d->taskRecvMulticastMetatraffic.mb.cdrCodec);
    CDR_codec_release_buffer(&d->taskRecvMulticastUserdata.mb.cdrCodec);
  }
  if (!manager) {
    CDR_codec_release_buffer(&d->taskRecvUnicastUserdata.mb.cdrCodec);
  }
  CDR_codec_release_buffer(&d->taskRecvUnicastMetatraffic.mb.cdrCodec);
err_domainProp:
  if (!errno_save)
    errno_save = errno;
  pthread_rwlock_destroy(&d->patternEntry.lock);
  pthread_rwlock_destroy(&d->psEntry.subscriptionsLock);
  pthread_rwlock_destroy(&d->psEntry.publicationsLock);
  pthread_rwlock_destroy(&d->subscriptions.lock);
  pthread_rwlock_destroy(&d->publications.lock);
  pthread_mutex_destroy(&d->objectEntry.htimSendMutex);
  pthread_rwlock_destroy(&d->objectEntry.htimRootLock);
  pthread_rwlock_destroy(&d->objectEntry.objRootLock);
  FREE(d);
  errno = errno_save;
  return NULL;
}

/*****************************************************************************/
Boolean
ORTEDomainDestroy(ORTEDomain *d, Boolean manager)
{
  CSTWriter             *cstWriter;
  CSTReader             *cstReader;

  debug(30, 10) ("ORTEDomainDestroy: start\n");
  if (!d)
    return ORTE_FALSE;

  pthread_rwlock_wrlock(&d->objectEntry.objRootLock);
  pthread_rwlock_wrlock(&d->objectEntry.htimRootLock);
  appSelfParamChanged(d, ORTE_TRUE, ORTE_TRUE, ORTE_FALSE, ORTE_FALSE);
  pthread_rwlock_unlock(&d->objectEntry.htimRootLock);
  pthread_rwlock_unlock(&d->objectEntry.objRootLock);

  //Stoping threads
  ORTEDomainRecvThreadStop(&d->taskRecvUnicastMetatraffic);
  ORTEDomainRecvThreadStop(&d->taskRecvMulticastMetatraffic);
  ORTEDomainRecvThreadStop(&d->taskRecvUnicastUserdata);
  ORTEDomainRecvThreadStop(&d->taskRecvMulticastUserdata);
  ORTEDomainSendThreadStop(&d->taskSend);
  debug(30, 3) ("ORTEDomainDestroy: threads stoped\n");

  //CSTReaders and CSTWriters
  CSTWriterDelete(d, &d->writerApplicationSelf);
  CSTReaderDelete(d, &d->readerManagers);
  CSTReaderDelete(d, &d->readerApplications);
  if (manager) {
    CSTWriterDelete(d, &d->writerManagers);
    CSTWriterDelete(d, &d->writerApplications);
  } else {
    CSTWriterDelete(d, &d->writerPublications);
    CSTWriterDelete(d, &d->writerSubscriptions);
    CSTReaderDelete(d, &d->readerPublications);
    CSTReaderDelete(d, &d->readerSubscriptions);

    while ((cstWriter = CSTWriter_cut_first(&d->publications))) {
      CSTWriterDelete(d, cstWriter);
      FREE(cstWriter);
    }
    while ((cstReader = CSTReader_cut_first(&d->subscriptions))) {
      CSTReaderDelete(d, cstReader);
      FREE(cstReader);
    }
  }

  //objects in objectsEntry
  objectEntryDeleteAll(d, &d->objectEntry);
  debug(30, 3) ("ORTEDomainDestroy: deleted all objects\n");

  //Sockets
  sock_cleanup(&d->taskRecvUnicastMetatraffic.sock);
  sock_cleanup(&d->taskRecvMulticastMetatraffic.sock);
  sock_cleanup(&d->taskRecvUnicastUserdata.sock);
  sock_cleanup(&d->taskRecvMulticastUserdata.sock);
  sock_cleanup(&d->taskSend.sock);


  //Signals
  pthread_cond_destroy(&d->objectEntry.htimSendCond);
  pthread_mutex_destroy(&d->objectEntry.htimSendMutex);

  //rwLocks
  pthread_rwlock_destroy(&d->objectEntry.objRootLock);
  pthread_rwlock_destroy(&d->objectEntry.htimRootLock);
  pthread_rwlock_destroy(&d->publications.lock);
  pthread_rwlock_destroy(&d->subscriptions.lock);
  pthread_rwlock_destroy(&d->psEntry.publicationsLock);
  pthread_rwlock_destroy(&d->psEntry.subscriptionsLock);

  //TypeRegister
  ORTETypeRegisterDestroyAll(d);

  //Pattern
  ORTEDomainAppSubscriptionPatternDestroy(d);
  pthread_rwlock_destroy(&d->patternEntry.lock);

  //Release buffers
  CDR_codec_release_buffer(&d->taskRecvUnicastMetatraffic.mb.cdrCodec);
  CDR_codec_release_buffer(&d->taskRecvMulticastMetatraffic.mb.cdrCodec);
  CDR_codec_release_buffer(&d->taskRecvUnicastUserdata.mb.cdrCodec);
  CDR_codec_release_buffer(&d->taskRecvMulticastUserdata.mb.cdrCodec);
  CDR_codec_release_buffer(&d->taskSend.mb.cdrCodec);

  //Free domain instance
  FREE(d);

  debug(30, 10) ("ORTEDomainDestroy: finished\n");

  return ORTE_TRUE;
}
