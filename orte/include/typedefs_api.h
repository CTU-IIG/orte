/*
 *  $Id: typedefs_api.h,v 0.0.0.1       2003/08/21 
 *
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

#ifndef _TYPEDEFS_API_H
#define _TYPEDEFS_API_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ORTEDomain ORTEDomain;             //forward declaration
typedef struct CSTWriter ORTEPublication;         //forward declaration
typedef struct CSTReader ORTESubscription;        //forward declaration

typedef enum {
        PULLED           = 0x01,
        IMMEDIATE        = 0x02
      } SubscriptionMode;

typedef enum {
        BEST_EFFORTS     = 0x01,
        STRICT_RELIABLE  = 0x02
      } SubscriptionType;
      
typedef enum {
        NEW_DATA         = 0x01,
        DEADLINE         = 0x02
      } ORTERecvStatus;

typedef enum {
        NEED_DATA        = 0x01,
        CQL              = 0x02
      } ORTESendStatus;

typedef struct ORTEIFProp {
  int32_t        ifFlags;
  IPAddress      ipAddress;
} ORTEIFProp;

typedef struct ORTEMulticastProp {
  Boolean        enabled;
  unsigned char  ttl;
  Boolean        loopBackEnabled;
  IPAddress      ipAddress;
} ORTEMulticastProp;

typedef struct ORTECDRStream {
  char           *buffer;
  char           *bufferPtr;
  Boolean        needByteSwap;
  int            length;
} ORTECDRStream;

typedef void (*ORTETypeSerialize)(ORTECDRStream *cdr_stream, void *instance);
typedef void (*ORTETypeDeserialize)(ORTECDRStream *cdr_stream, void *instance);

/**
 * struct ORTETypeRegister - 
 */
typedef struct ORTETypeRegister {
  char                   *typeName;
  ORTETypeSerialize      serialize;
  ORTETypeDeserialize    deserialize;
  unsigned int           getMaxSize;
} ORTETypeRegister;

/**
 * struct ORTEDomainBaseProp - 
 */
typedef struct ORTEDomainBaseProp {
  NtpTime                expirationTime;
  NtpTime                refreshPeriod;
  NtpTime                purgeTime;
  NtpTime                repeatAnnounceTime;
  NtpTime                repeatActiveQueryTime;
  NtpTime                delayResponceTimeACKMin;
  NtpTime                delayResponceTimeACKMax;
  unsigned int           HBMaxRetries;
  unsigned int           ACKMaxRetries;
  NtpTime                maxBlockTime;
} ORTEDomainBaseProp;

/**
 * struct ORTEDomainWireProp - 
 */
typedef struct ORTEDomainWireProp {
  unsigned int           metaBytesPerPacket;
  unsigned int           metaBytesPerFastPacket;
  unsigned int           metabitsPerACKBitmap;
  unsigned int           userMaxSerDeserSize;
} ORTEDomainWireProp;

/**
 * struct ORTEPublProp - 
 */
typedef struct ORTEPublProp {
  PathName               topic;
  TypeName               typeName; 
  TypeChecksum           typeChecksum;
  Boolean                expectsAck;
  NtpTime                persistence;
  u_int32_t              reliabilityOffered;
  u_int32_t              sendQueueSize;
  int32_t                strength;
  //Additional properties
  u_int32_t              criticalQueueLevel;
  NtpTime                HBNornalRate;
  NtpTime                HBCQLRate;
  unsigned int           HBMaxRetries;
  //
  NtpTime                maxBlockTime;
} ORTEPublProp;

/**
 * struct ORTESubsProp - 
 */
typedef struct ORTESubsProp {
  PathName               topic;
  TypeName               typeName;
  TypeChecksum           typeChecksum;
  NtpTime                minimumSeparation;
  u_int32_t              recvQueueSize;
  u_int32_t              reliabilityRequested;
  //additional parameters
  NtpTime                deadline;
  u_int32_t              mode;
}  ORTESubsProp;

/**
 * struct ORTEAppInfo - 
 */
typedef struct ORTEAppInfo {
  HostId                 hostId;
  AppId                  appId;
  IPAddress              *unicastIPAddressList;
  unsigned char          unicastIPAddressCount;
  IPAddress              *metatrafficMulticastIPAddressList;
  unsigned char          metatrafficMulticastIPAddressCount;
  Port                   metatrafficUnicastPort;
  Port                   userdataUnicastPort;
  VendorId               vendorId;
  ProtocolVersion        protocolVersion;
} ORTEAppInfo;

/**
 * struct ORTEPubInfo - 
 */
typedef struct ORTEPubInfo {
  const char            *topic;
  const char            *type;
  ObjectId              objectId;
} ORTEPubInfo;

/**
 * struct ORTEPubInfo - 
 */
typedef struct ORTESubInfo {
  const char            *topic;
  const char            *type;
  ObjectId              objectId;
} ORTESubInfo;

/**
 * struct ORTEPublStatus - status of a publication
 * @reliable: count of reliable subscriptions (best effort) connected on responsible publication
 * @unReliable: count of unreliable subscriptions (strict) connected on responsible publication
 * @issues: number of messages in sending queue
 * @unacknowledgedIssues: number of unacknowledged issues (only for best effort)
 */
typedef struct ORTEPublStatus {
  unsigned int           strict;
  unsigned int           bestEffort;
  unsigned int           issues;
} ORTEPublStatus;

/**
 * struct ORTESubsStatus - status of a subscription
 * @reliable: count of reliable publications (best effort) connected to responsible subscription
 * @unReliable: count of unreliable publications (strict) connected to responsible subscription
 * @issues: number of messages in receiving queue
 *
 * Current implementation has always issues=0. It means, that all messages were sent to user application
 */
typedef struct ORTESubsStatus {
  unsigned int           strict;
  unsigned int           bestEffort;
  unsigned int           issues;
} ORTESubsStatus;

/**
 * struct ORTERecvInfo - 
 */
typedef struct ORTERecvInfo {
  ORTERecvStatus        status;
  const char            *topic;
  const char            *type;
  GUID_RTPS             senderGUID;
  NtpTime               localTimeReceived;
  NtpTime               remoteTimePublished;
  SequenceNumber        sn;
} ORTERecvInfo;

/**
 * struct ORTESendInfo - 
 */
typedef struct ORTESendInfo {
  ORTESendStatus        status;
  const char            *topic;
  const char            *type;
  GUID_RTPS             senderGUID;
  SequenceNumber        sn;
} ORTESendInfo;

//CallBackRutines
typedef void 
(*ORTERecvCallBack)(const ORTERecvInfo *info,void *instance, void *recvCallBackParam);
typedef void 
(*ORTESendCallBack)(const ORTESendInfo *info,void *instance, void *sendCallBackParam);
typedef ORTESubscription*
(*ORTESubscriptionPatternCallBack)(const char *topic, const char *nddsType, void *Param);


//Pattern
typedef Boolean
(*ORTEPatternCheck)(const char *string);
typedef Boolean
(*ORTEPatternMatch)(const char *topic,const char *pattern,void *param);
extern Boolean
ORTEPatternCheckDefault(const char *topic);
extern Boolean
ORTEPatternMatchDefault(const char *topic,const char *pattern,void *param);
extern void
ORTEPatternRegister(ORTEDomain *d,ORTEPatternCheck check,
     ORTEPatternMatch match,void *param);

//Manager
typedef Boolean 
(*ORTEOnMgrNew)(const struct ORTEAppInfo *appInfo, void *param);
typedef void 
(*ORTEOnMgrDelete)(const struct ORTEAppInfo *appInfo, void *param);
//Application
typedef Boolean 
(*ORTEOnAppRemoteNew)(const struct ORTEAppInfo *appInfo, void *param);
typedef void (*ORTEOnAppDelete)
(const struct ORTEAppInfo *appInfo, void *param);
//Publication
typedef Boolean 
(*ORTEOnPubRemote)(const struct ORTEAppInfo *appInfo,
                   const struct ORTEPubInfo *pubInfo, void *param);
typedef void 
(*ORTEOnPubDelete)(const struct ORTEAppInfo *appInfo,
                   const struct ORTEPubInfo *pubInfo, void *param);
//Subscription
typedef Boolean 
(*ORTEOnSubRemote)(const struct ORTEAppInfo *appInfo,
                   const struct ORTESubInfo *subInfo, void *param);
typedef void 
(*ORTEOnSubDelete)(const struct ORTEAppInfo *appInfo,
                   const struct ORTESubInfo *subInfo, void *param);

/**
 * struct ORTEDomainAppEvents - Domain events of an application
 */
typedef struct ORTEDomainAppEvents {
  ORTEOnMgrNew           onMgrNew;
  void                   *onMgrNewParam;
  ORTEOnMgrDelete        onMgrDelete;
  void                   *onMgrDeleteParam;
  ORTEOnAppRemoteNew     onAppRemoteNew;
  void                   *onAppRemoteNewParam;
  ORTEOnAppDelete        onAppDelete;
  void                   *onAppDeleteParam;
  ORTEOnPubRemote        onPubRemoteNew;
  void                   *onPubRemoteNewParam;
  ORTEOnPubRemote        onPubRemoteChanged;
  void                   *onPubRemoteChangedParam;
  ORTEOnPubDelete        onPubDelete;
  void                   *onPubDeleteParam;
  ORTEOnSubRemote        onSubRemoteNew;
  void                   *onSubRemoteNewParam;
  ORTEOnSubRemote        onSubRemoteChanged;
  void                   *onSubRemoteChangedParam;
  ORTEOnSubDelete        onSubDelete;
  void                   *onSubDeleteParam;
} ORTEDomainAppEvents;

/**
 * struct ORTETaskProp - 
 */
typedef struct ORTETasksProp {
  Boolean                realTimeEnabled;
  int                    smtStackSize;
  int                    smtPriority;
  int                    rmtStackSize;
  int                    rmtPriority;
} ORTETasksProp; 

/**
 * struct ORTEDomainApp - 
 */
typedef struct ORTEDomainProp {
  ORTETasksProp          tasksProp;
  ORTEIFProp             IFProp[MAX_INTERFACES];  //interface properties
  unsigned char          IFCount;                 //count of interfaces
  ORTEDomainBaseProp     baseProp;
  ORTEDomainWireProp     wireProp;
  ORTEMulticastProp      multicast;               //multicast properies
  char                   *mgrs;                   //managers
  IPAddress              mgrAddKey;               //keys
  IPAddress              appLocalManager;         //applications
  char                   version[60];             //string product version
  int                    recvBuffSize;
  int                    sendBuffSize;      
} ORTEDomainProp;

#ifdef __cplusplus
} /* extern "C"*/
#endif

#endif  /* _TYPEDEFS_API_H */

