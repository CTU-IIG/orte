/*
 *  $Id: protos.h,v 0.0.0.1             2003/08/21 
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

#ifndef _PROTOS_H
#define _PROTOS_H

#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// sock.c
extern int
sock_start(void);
extern void
sock_finish(void);
extern int
sock_init_udp(sock_t *sock);
extern void
sock_cleanup(sock_t *sock);
extern int
sock_setsockopt(sock_t *sock,int optname,const char *optval, int optlen);
extern int
sock_getsockopt(sock_t *sock,int optname,char *optval, int *optlen);
extern int
sock_bind(sock_t *sock,u_int16_t port);
extern int
sock_recvfrom(sock_t *sock, void *buf, int max_len,struct sockaddr_in *des,int des_len);
extern int
sock_sendto(sock_t *sock, void *buf, int len,struct sockaddr_in *des,int des_len);
extern int
sock_ioctl(sock_t *sock, long cmd, unsigned long *arg);
extern int
sock_get_local_interfaces(sock_t *sock,ORTEIFProp *IFProp,char *IFCount);

///////////////////////////////////////////////////////////////////////////////
// conv.c
extern void 
conv_u16(u_int16_t *x,char ef);
extern void 
conv_u32(u_int32_t *x,char ef);
extern void 
conv_sn(SequenceNumber *sn,char ef);
extern void 
conv_ntp(NtpTime *ntp,char ef);
extern int
getStringPart(char *string,char divChar,int *iterator,char *buff);

///////////////////////////////////////////////////////////////////////////////
// htimerNtp.c
// htimerRoot
UL_HTIMER_DEC(htimerRoot, \
      ObjectEntry, \
      HTimFncRootNode, \
      htimRoot, \
      htim) 
extern void
htimerRoot_run_expired(ORTEDomain *d, 
      ul_htim_time_t *pact_time);
// htimerUnicastCommon
UL_HTIMER_DEC(htimerUnicastCommon, \
      ObjectEntryAID, \
      HTimFncUserNode, \
      htimUnicast.common, \
      htim) 
extern void
htimerUnicastCommon_update_root_timer(ObjectEntry *objectEntry,
      ObjectEntryAID *objectEntryAID);
extern void 
htimerUnicastCommon_run_expired(ORTEDomain *d,
      ObjectEntryAID *objectEntryAID, 
      ul_htim_time_t *pact_time);
// htimerUnicastSendMetatraffic
UL_HTIMER_DEC(htimerUnicastSendMetatraffic, \
      ObjectEntryAID, \
      HTimFncUserNode, \
      htimUnicast.sendMetatraffic, \
      htim) 
extern void
htimerUnicastSendMetatraffic_update_root_timer(ObjectEntry *objectEntry,
      ObjectEntryAID *objectEntryAID);
extern void 
htimerUnicastSendMetatraffic_run_expired(ORTEDomain *d,
      ObjectEntryAID *objectEntryAID, 
      ul_htim_time_t *pact_time);
// htimerUnicastSendUserData
UL_HTIMER_DEC(htimerUnicastSendUserData, \
      ObjectEntryAID, \
      HTimFncUserNode, \
      htimUnicast.sendUserData, \
      htim) 
extern void
htimerUnicastSendUserData_update_root_timer(ObjectEntry *objectEntry,
      ObjectEntryAID *objectEntryAID);
extern void 
htimerUnicastSendUserData_run_expired(ORTEDomain *d,
      ObjectEntryAID *objectEntryAID, 
      ul_htim_time_t *pact_time);
extern NtpTime
getActualNtpTime(void);


///////////////////////////////////////////////////////////////////////////////
// debug.c
extern void
db_init(const char *logfile, const char *options);
extern void
debug_options(const char *options);
extern void
db_print(const char *format,...);
extern void
debug_open_log(const char *logfile);
#ifdef ENABLE_MEM_CHECK
extern void * 
mem_check_malloc(size_t size);
extern void 
mem_check_free(void *ptr);
#endif

///////////////////////////////////////////////////////////////////////////////
// fnmatch.c
extern int
fnmatch(const char *__pattern,const char *__string,int __flags);

///////////////////////////////////////////////////////////////////////////////
// objectEntry.c
GAVL_CUST_NODE_INT_DEC(ObjectEntryHID, ObjectEntry, ObjectEntryHID, HostId,
    objRoot, hidNode, hid, gavl_cmp_int)
GAVL_CUST_NODE_INT_DEC(ObjectEntryAID, ObjectEntryHID, ObjectEntryAID, AppId,
    aidRoot, aidNode, aid, gavl_cmp_int)
GAVL_CUST_NODE_INT_DEC(ObjectEntryOID, ObjectEntryAID, ObjectEntryOID, ObjectId,
    oidRoot, oidNode, oid, gavl_cmp_int)
extern void
objectEntryRefreshApp(ORTEDomain *d,ObjectEntryOID *objectEntryOID);
extern ObjectEntryOID *
objectEntryFind(ORTEDomain *d,GUID_RTPS *guid);
extern ObjectEntryOID *
objectEntryAdd(ORTEDomain *d,GUID_RTPS *guid,void *params);
extern int
objectEntryDelete(ORTEDomain *d,ObjectEntryOID *objectEntryOID);
extern void
objectEntryDeleteAll(ORTEDomain *d,ObjectEntry *objectEntry);
extern void
objectEntryDump(ObjectEntry *objectEntry);

///////////////////////////////////////////////////////////////////////////////
// ObjectUtils.c
extern void 
AppParamsInit(AppParams *ap);
extern void 
PublParamsInit(ORTEPublProp *sp);
extern void 
SubsParamsInit(ORTESubsProp *sp);
extern void
updateORTEAppInfo(AppParams *appParams,ORTEAppInfo *appInfo);
extern Boolean
generateEvent(ORTEDomain *d,GUID_RTPS *guid,void *params,Boolean live);
extern Boolean
getTypeApp(ORTEDomain *d,AppParams *ap,IPAddress senderIPAddress);
extern void 
appSelfParamChanged(ORTEDomain *d,Boolean lock,Boolean unlock,
   Boolean forWM,Boolean alive);

///////////////////////////////////////////////////////////////////////////////
// event.c
extern int32_t 
eventAdd(ORTEDomain *d,ObjectEntryAID *objectEntryAID,HTimFncUserNode *fncNode,
         int where,const char *name,EVH2 *func,pthread_rwlock_t *lock,
         void *arg1,NtpTime *when_expire);
extern int32_t
eventDetach(ORTEDomain *d,               
            ObjectEntryAID *objectEntryAID,
            HTimFncUserNode *fncNode,
            int where);

///////////////////////////////////////////////////////////////////////////////
// parameter.c
UL_LIST_CUST_DEC(CSChangeAttributes,
                 CSChange,ParameterSequence,
                 attributes,node);
extern int
parameterDelete(CSChange *csChange);
extern int
parameterCodeStreamFromCSChange(CSChange *csChange,
    u_int8_t *rtps_msg,u_int32_t max_msg_len);
extern int
parameterDecodeStreamToCSChange(CSChange *csChange,u_int8_t *rtps_msg,
    u_int16_t submsg_len,u_int8_t needByteSwap);
extern int
parameterUpdateCSChange(
     CSChange *csChange,AppParams *ap,Boolean Manager);
extern int
parameterUpdateCSChangeFromPublication(CSChange *csChange,ORTEPublProp *pp);
extern int
parameterUpdateCSChangeFromSubscription(CSChange *csChange,ORTESubsProp *sp);
extern int
parameterUpdateApplication(CSChange *csChange,AppParams *ap);
extern int
parameterUpdatePublication(CSChange *csChange,ORTEPublProp *pp);
extern int
parameterUpdateSubscription(CSChange *csChange,ORTESubsProp *sp);

///////////////////////////////////////////////////////////////////////////////
// objectEntryTimer.c
extern int
objectEntryPurgeTimer(ORTEDomain *d,void *vobjectEntryOID);
extern int
objectEntryExpirationTimer(ORTEDomain *d,void *vobjectEntryOID);

///////////////////////////////////////////////////////////////////////////////
// ORTETypeRegister.c
GAVL_CUST_NODE_INT_DEC(ORTEType, TypeEntry, TypeNode, const char *,
    types, node, typeRegister.typeName, gavl_cmp_str)


///////////////////////////////////////////////////////////////////////////////
// ORTEThreadUtils.c
extern void
ORTEDomainWakeUpReceivingThread(ORTEDomain *d,sock_t *sock,u_int16_t port);
extern void
ORTEDomainWakeUpSendingThread(ObjectEntry *objectEntry);

///////////////////////////////////////////////////////////////////////////////
// ORTEAppRecvMetatrafficThread.c
extern void 
ORTEAppRecvMetatrafficThread(ORTEDomain *d);

///////////////////////////////////////////////////////////////////////////////
// ORTEAppRecvUserdataThread.c
extern void 
ORTEAppRecvUserdataThread(ORTEDomain *d);

///////////////////////////////////////////////////////////////////////////////
// ORTEAppSendThread.c
extern void 
ORTESendData(ORTEDomain *d,ObjectEntryAID *objectEntryAID,Boolean meta);

///////////////////////////////////////////////////////////////////////////////
// RTPSHeader.c
extern int16_t 
RTPSHeaderCreate(u_int8_t *msg,HostId hid,AppId aid);
extern int16_t 
RTPSHeaderCheck(u_int8_t *msg,int32_t len,MessageInterpret *mi);

///////////////////////////////////////////////////////////////////////////////
// RTPSPad.c
extern void 
RTPSPad(u_int8_t *rtps_msg,MessageInterpret *mi);

///////////////////////////////////////////////////////////////////////////////
// RTPSInfoDST.c
extern void 
RTPSInfoDST(u_int8_t *rtps_msg,MessageInterpret *mi);

///////////////////////////////////////////////////////////////////////////////
// RTPSInfoREPLY.c
extern void 
RTPSInfoREPLY(u_int8_t *rtps_msg,MessageInterpret *mi);

///////////////////////////////////////////////////////////////////////////////
// RTPSInfoREPLY.c
extern int32_t 
RTPSInfoREPLYCreate(u_int8_t *rtps_msg,u_int32_t max_msg_len,
    IPAddress ipaddress,Port port);
extern void 
RTPSInfoSRC(u_int8_t *rtps_msg,MessageInterpret *mi);

///////////////////////////////////////////////////////////////////////////////
// RTPSInfoTS.c
extern int32_t 
RTPSInfoTSCreate(u_int8_t *rtps_msg,u_int32_t max_msg_len,NtpTime time);
extern void 
RTPSInfoTS(u_int8_t *rtps_msg,MessageInterpret *mi);

///////////////////////////////////////////////////////////////////////////////
// RTPSVar.c
extern void 
RTPSVar(ORTEDomain *d,u_int8_t *rtps_msg,MessageInterpret *mi,IPAddress senderIPAddress);

///////////////////////////////////////////////////////////////////////////////
// RTPSAck.c
extern int32_t 
RTPSAckCreate(u_int8_t *rtps_msg,u_int32_t max_msg_len,
    SequenceNumber *seqNumber,
    ObjectId roid,ObjectId woid,Boolean f_bit);
extern void 
RTPSAck(ORTEDomain *d,u_int8_t *rtps_msg,MessageInterpret *mi,IPAddress senderIPAddress);

///////////////////////////////////////////////////////////////////////////////
// RTPSHeardBeat.c
extern int 
RTPSHeardBeatCreate(u_int8_t *rtps_msg,u_int32_t max_msg_len,
    SequenceNumber *firstSeqNumber,SequenceNumber *lastSeqNumber,
    ObjectId woid,ObjectId roid,Boolean f_bit);
extern void 
RTPSHeardBeat(ORTEDomain *d,u_int8_t *rtps_msg,MessageInterpret *mi);

///////////////////////////////////////////////////////////////////////////////
// RTPSGap.c
extern void 
RTPSGap(ORTEDomain *d,u_int8_t *rtps_msg,MessageInterpret *mi,IPAddress senderIPAddress);

///////////////////////////////////////////////////////////////////////////////
// RTPSIssue.c
extern int32_t
RTPSIssueCreateHeader(u_int8_t *rtps_msg,u_int32_t max_msg_len,u_int32_t length,
    ObjectId roid,ObjectId woid,SequenceNumber sn);
extern void 
RTPSIssue(ORTEDomain *d,u_int8_t *rtps_msg,MessageInterpret *mi,IPAddress senderIPAddress);

///////////////////////////////////////////////////////////////////////////////
// RTPSUtils.c
extern int gavl_cmp_ntp_time(const NtpTime *a,const NtpTime *b);
extern int gavl_cmp_sn(const SequenceNumber *a, const SequenceNumber *b); 
extern int gavl_cmp_guid(const GUID_RTPS *a, const GUID_RTPS *b); 
extern int gavl_cmp_str(const char *const *a,const char *const *b);
extern int getMaxMessageLength(ORTEDomain *d);

///////////////////////////////////////////////////////////////////////////////
// RTPSCSTWriter.c
UL_LIST_CUST_DEC(CSTWriterCSChange,
                 CSTWriter,CSChange,
                 csChanges,nodeList);
GAVL_CUST_NODE_INT_DEC(CSTWriter, 
                       CSTPublications, CSTWriter, GUID_RTPS,
                       cstWriter, node, guid, gavl_cmp_guid);
GAVL_CUST_NODE_INT_DEC(CSTRemoteReader, 
                       CSTWriter, CSTRemoteReader, GUID_RTPS,
                       cstRemoteReader, node, guid, gavl_cmp_guid);
GAVL_CUST_NODE_INT_DEC(CSChangeForReader,
                       CSTRemoteReader, CSChangeForReader, SequenceNumber,
                       csChangeForReader, node, csChange->sn, gavl_cmp_sn);

extern void
CSTWriterInit(ORTEDomain *d,CSTWriter *cstWriter,ObjectEntryOID *object,
    ObjectId oid,CSTWriterParams *params,ORTETypeRegister *typeRegister);
extern void 
CSTWriterDelete(ORTEDomain *d,CSTWriter *cstWriter);
extern void
CSTWriterAddRemoteReader(ORTEDomain *d,CSTWriter *cstWriter,ObjectEntryOID *object,
    ObjectId oid);
extern void 
CSTWriterDestroyRemoteReader(ORTEDomain *d,CSTRemoteReader *cstRemoteReader);
extern void
CSTWriterMakeGAP(ORTEDomain *d,CSTWriter *cstWriter,GUID_RTPS *guid);
extern void
CSTWriterAddCSChange(ORTEDomain *d,CSTWriter *cstWriter,CSChange *csChange);
extern void
CSTWriterDestroyCSChangeForReader(CSTRemoteReader *cstRemoteReader,
    CSChangeForReader   *csChangeForReader,Boolean destroyCSChange);
extern void 
CSTWriterDestroyCSChange(ORTEDomain *d,CSTWriter *cstWriter,CSChange *csChange);
extern Boolean
CSTWriterTryDestroyBestEffortIssue(CSTWriter *cstWriter);
extern void
CSTWriterRefreshAllCSChanges(ORTEDomain *d,CSTRemoteReader *cstRemoteReader);

///////////////////////////////////////////////////////////////////////////////
// RTPSCSTWriterTimer.c
extern int
CSTWriterRefreshTimer(ORTEDomain *d,void *vcstWriter);
extern int 
CSTWriterAnnounceTimer(ORTEDomain *d,void *vcstRemoteReader);
extern int 
CSChangeForReaderUnderwayTimer(ORTEDomain *da,void *vcsChangeForReader);
extern int
CSTWriterSendBestEffortTimer(ORTEDomain *d,void *vcstRemoteReader);
extern int
CSTWriterSendStrictTimer(ORTEDomain *d,void *vcstRemoteReader);
extern int
CSTWriterSendTimer(ORTEDomain *d,void *vcstRemoteReader);
        
///////////////////////////////////////////////////////////////////////////////
// RTPSCSTReader.c
UL_LIST_CUST_DEC(CSTReaderCSChange,
                 CSTReader,CSChange,
                 csChanges,nodeList);
GAVL_CUST_NODE_INT_DEC(CSTReader, 
                       CSTSubscriptions, CSTReader, GUID_RTPS,
                       cstReader, node, guid, gavl_cmp_guid);
GAVL_CUST_NODE_INT_DEC(CSTRemoteWriter, 
                       CSTReader, CSTRemoteWriter, GUID_RTPS,
                       cstRemoteWriter, node, guid, gavl_cmp_guid);
GAVL_CUST_NODE_INT_DEC(CSChangeFromWriter,
                       CSTRemoteWriter, CSChangeFromWriter, SequenceNumber,
                       csChangeFromWriter, node, csChange->sn, gavl_cmp_sn);
extern void 
CSTReaderInit(ORTEDomain *d,CSTReader *cstReader,ObjectEntryOID *object,
    ObjectId oid,CSTReaderParams *params,ORTETypeRegister *typeRegister);
extern void 
CSTReaderDelete(ORTEDomain *d,CSTReader *cstReader);
extern CSTRemoteWriter *
CSTReaderAddRemoteWriter(ORTEDomain *d,CSTReader *cstReader,ObjectEntryOID *object,
    ObjectId oid);
extern void 
CSTReaderDestroyRemoteWriter(ORTEDomain *d,CSTRemoteWriter *cstRemoteWriter);
extern void
CSTReaderAddCSChange(CSTRemoteWriter *cstRemoteWriter,CSChange *csChange);
extern void
CSTReaderDestroyCSChangeFromWriter(CSTRemoteWriter *cstRemoteWriter,
    CSChangeFromWriter *csChangeFromWriter,Boolean keepCSChange);
extern void 
CSTReaderDestroyCSChange(CSTRemoteWriter *cstRemoteWriter,SequenceNumber *sn,
    Boolean keepCSChange);
extern void 
CSTReaderSetupState(CSTRemoteWriter *cstRemoteWriter);
        
///////////////////////////////////////////////////////////////////////////////
// RTPSCSTReaderProc.c
extern void 
CSTReaderProcCSChanges(ORTEDomain *d,CSTRemoteWriter *cstRemoteWriter);
extern void 
CSTReaderProcCSChangesIssue(CSTRemoteWriter *cstRemoteWriter,Boolean pullCalled);

///////////////////////////////////////////////////////////////////////////////
// RTPSCSTReaderTimer.c
extern int
CSTReaderResponceTimer(ORTEDomain *d,void *vcstRemoteWriter);
extern int 
CSTReaderQueryTimer(ORTEDomain *d,void *vcstRemoteWriter);
extern int 
CSTReaderDeadlineTimer(ORTEDomain *d,void *vcstReader);
extern int 
CSTReaderPersistenceTimer(ORTEDomain *d,void *vcstReader);

///////////////////////////////////////////////////////////////////////////////
// ORTEDomainApp.c
UL_LIST_CUST_DEC(Pattern,
                 PatternEntry,PatternNode,
                 patterns,node);
                                                   
///////////////////////////////////////////////////////////////////////////////
// ORTETypeRegister.c
extern Boolean
ORTETypeRegisterFind(ORTEDomain *d,const char *typeName);

///////////////////////////////////////////////////////////////////////////////
// ORTEPublication.c
GAVL_CUST_NODE_INT_DEC(PublicationList, 
                       PSEntry, ObjectEntryOID, GUID_RTPS,
                       publications, psNode, guid, gavl_cmp_guid);
extern int
ORTEPublicationSendLocked(ORTEPublication *cstWriter);

///////////////////////////////////////////////////////////////////////////////
// ORTESubcription.c
extern int
ORTESubscriptionDestroyLocked(ORTESubscription *cstReader);

///////////////////////////////////////////////////////////////////////////////
// ORTEPublicationTimer.c
extern int
PublicationCallBackTimer(ORTEDomain *d,void *vcstWriter);

///////////////////////////////////////////////////////////////////////////////
// ORTESubscription.c
GAVL_CUST_NODE_INT_DEC(SubscriptionList, 
                       PSEntry, ObjectEntryOID, GUID_RTPS,
                       subscriptions, psNode, guid, gavl_cmp_guid);

                       
#ifdef __cplusplus
} /* extern "C"*/
#endif

#endif /* _PROTOS_H */
