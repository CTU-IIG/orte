/*
 *  $Id: protos_api.h,v 0.0.0.1             2003/09/10 
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

#ifndef _PROTOS_API_H
#define _PROTOS_API_H

#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// conv.c
extern char* 
IPAddressToString(IPAddress ipAddress);
extern IPAddress
StringToIPAddress(const char *string);

///////////////////////////////////////////////////////////////////////////////
// ORTEDomain.c
extern Boolean
ORTEDomainPropDefaultGet(ORTEDomainProp *prop);
extern Boolean
ORTEDomainInitEvents(ORTEDomainAppEvents *events);
extern Boolean
ORTEPublPropDefaultGet(ORTEDomain *d,ORTEPublProp *pp);
extern Boolean
ORTEPublPropDefaultSet(ORTEDomain *d,ORTEPublProp *pp);
extern Boolean
ORTESubsPropDefaultGet(ORTEDomain *d,ORTESubsProp *sp);
extern Boolean
ORTESubsPropDefaultSet(ORTEDomain *d,ORTESubsProp *sp);

///////////////////////////////////////////////////////////////////////////////
// ORTEDomainApp.c
extern ORTEDomain * 
ORTEDomainAppCreate(int domain,ORTEDomainProp *prop,ORTEDomainAppEvents *events);
extern Boolean
ORTEDomainAppDestroy(ORTEDomain *d);
extern Boolean 
ORTEDomainAppSubscriptionPatternAdd(ORTEDomain *d,const char *topic,
    const char *type,ORTESubscriptionPatternCallBack subscriptionCallBack, 
    void *param);
extern Boolean 
ORTEDomainAppSubscriptionPatternRemove(ORTEDomain *d,const char *topic,
    const char *type);
extern Boolean 
ORTEDomainAppSubscriptionPatternDestroyAll(ORTEDomain *d);

///////////////////////////////////////////////////////////////////////////////
// ORTEDomainMgr.c
extern ORTEDomain *
ORTEDomainMgrCreate(int domain, ORTEDomainProp *prop,
                    ORTEDomainAppEvents *events,Boolean startSendingThread);
extern Boolean
ORTEDomainMgrDestroy(ORTEDomain *d);

///////////////////////////////////////////////////////////////////////////////
// ORTEPublication.c
extern ORTEPublication * 
ORTEPublicationCreate(ORTEDomain *d,char *topic,char *typeName,
    void *instance,NtpTime *persistence,int strength,
    ORTESendCallBack sendCallBack,void *sendCallBackParam,
    NtpTime *sendCallBackDelay);
extern int
ORTEPublicationDestroy(ORTEPublication *cstWriter);
extern int
ORTEPublicationPropertiesGet(ORTEPublication *cstWriter,ORTEPublProp *pp);
extern int
ORTEPublicationPropertiesSet(ORTEPublication *cstWriter,ORTEPublProp *pp);
extern int
ORTEPublicationWaitForSubscriptions(ORTEPublication *cstWriter,NtpTime wait,
    unsigned int retries,u_int32_t noSubscriptions);
extern int
ORTEPublicationGetStatus(ORTEPublication *cstWriter,ORTEPublStatus *status);
extern int 
ORTEPublicationSend(ORTEPublication *cstWriter);

///////////////////////////////////////////////////////////////////////////////
// ORTESubscription.c
extern ORTESubscription * 
ORTESubscriptionCreate(ORTEDomain *d,SubscriptionMode mode,SubscriptionType sType,
    char *topic,char *typeName,void *instance,NtpTime *deadline,
    NtpTime *minimumSeparation,ORTERecvCallBack recvCallBack,
    void *recvCallBackParam);
extern int
ORTESubscriptionDestroy(ORTESubscription *cstReader);
extern int
ORTESubscriptionPropertiesGet(ORTESubscription *cstReader,ORTESubsProp *sp);
extern int
ORTESubscriptionPropertiesSet(ORTESubscription *cstReader,ORTESubsProp *sp);
extern int
ORTESubscriptionWaitForPublications(ORTESubscription *cstReader,NtpTime wait,
    unsigned int retries,unsigned int noPublications);
extern int
ORTESubscriptionGetStatus(ORTESubscription *cstReader,ORTESubsStatus *status);
extern int
ORTESubscriptionPull(ORTESubscription *cstReader);

///////////////////////////////////////////////////////////////////////////////
// ORTETypeRegister.c
extern int
ORTETypeRegisterAdd(ORTEDomain *d,char *typeName,ORTETypeSerialize ts,
                    ORTETypeDeserialize ds,unsigned int gms);
extern int
ORTETypeRegisterDestroyAll(ORTEDomain *d);

///////////////////////////////////////////////////////////////////////////////
// ORTEVerbosity.c
extern void 
ORTEVerbositySetOptions(const char *options);
extern void 
ORTEVerbositySetLogFile(const char *arg);


///////////////////////////////////////////////////////////////////////////////
// ORTEInit.c
extern void
ORTEInit(void);

///////////////////////////////////////////////////////////////////////////////
// ORTEAppSendThread.c
extern void 
ORTEAppSendThread(ORTEDomain *d);

#ifdef __cplusplus
} /* extern "C"*/
#endif

#endif /* _PROTOS_API_H */
