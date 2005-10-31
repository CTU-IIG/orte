/*
	ortedll.h

	Historie zmen:
		2005-08-23	vytvoril	Milan
*/

#pragma once

#include <windows.h>
#include <orte.h>

#ifdef __cplusplus
	extern "C" {
#endif

typedef void (*PFN_ORTEInit)(void);

typedef void (*PFN_ORTEDomainStart)(ORTEDomain, Boolean, Boolean, Boolean,
	Boolean, Boolean);

typedef Boolean (*PFN_ORTEDomainPropDefaultGet)(ORTEDomainProp *);

typedef Boolean (*PFN_ORTEDomainInitEvents)(ORTEDomainAppEvents *);

typedef ORTEDomain* (*PFN_ORTEDomainAppCreate)(int, ORTEDomainProp *,
	ORTEDomainAppEvents *, Boolean suspended);

typedef Boolean (*PFN_ORTEDomainAppDestroy)(ORTEDomain *);

typedef Boolean (*PFN_ORTEDomainAppSubscriptionPatternAdd)(ORTEDomain *,
	const char *, const char *, ORTESubscriptionPatternCallBack, 
	void *);

typedef Boolean (*PFN_ORTEDomainAppSubscriptionPatternRemove)(ORTEDomain *,
	const char *, const char *);

typedef Boolean (*PFN_ORTEDomainAppSubscriptionPatternDestroy)(ORTEDomain *);

typedef ORTEDomain* (*PFN_ORTEDomainMgrCreate)(int, ORTEDomainProp *,
	ORTEDomainAppEvents *, Boolean);

typedef Boolean (*PFN_ORTEDomainMgrDestroy)(ORTEDomain *);

typedef ORTEPublication* (*PFN_ORTEPublicationCreate)(ORTEDomain *,
	const char *, const char *, void *, NtpTime *, int,
	ORTESendCallBack, void *, NtpTime *);

typedef int (*PFN_ORTEPublicationDestroy)(ORTEPublication *);

typedef int (*PFN_ORTEPublicationPropertiesGet)(ORTEPublication *,
	ORTEPublProp *);

typedef int (*PFN_ORTEPublicationPropertiesSet)(ORTEPublication *,
	ORTEPublProp *);

typedef int (*PFN_ORTEPublicationWaitForSubscriptions)(
	ORTEPublication *, NtpTime, unsigned int, unsigned int);

typedef int (*PFN_ORTEPublicationGetStatus)(ORTEPublication *,
	ORTEPublStatus *);

typedef int (*PFN_ORTEPublicationSend)(ORTEPublication *);

typedef int (*PFN_ORTEPublicationSendEx)(ORTEPublication *,
	ORTEPublicationSendParam *);

typedef ORTESubscription* (*PFN_ORTESubscriptionCreate)(ORTEDomain *,
	SubscriptionMode, SubscriptionType, const char *, const char *,
	void *, NtpTime *, NtpTime *, ORTERecvCallBack, void *,
	IPAddress);

typedef int (*PFN_ORTESubscriptionDestroy)(ORTESubscription *);

typedef int (*PFN_ORTESubscriptionPropertiesGet)(ORTESubscription *,
	ORTESubsProp *);

typedef int (*PFN_ORTESubscriptionPropertiesSet)(ORTESubscription *,
	ORTESubsProp *);

typedef int (*PFN_ORTESubscriptionWaitForPublications)(ORTESubscription *,
	NtpTime, unsigned int,unsigned int);

typedef int (*PFN_ORTESubscriptionGetStatus)(ORTESubscription *,
	ORTESubsStatus *);

typedef int (*PFN_ORTESubscriptionPull)(ORTESubscription *);

typedef int (*PFN_ORTETypeRegisterAdd)(ORTEDomain *, const char *,
	ORTETypeSerialize, ORTETypeDeserialize, ORTETypeGetMaxSize,
	unsigned int);

typedef int (*PFN_ORTETypeRegisterDestroyAll)(ORTEDomain *);

typedef void (*PFN_ORTEVerbositySetOptions)(const char *);

typedef void (*PFN_ORTEVerbositySetLogFile)(const char *);

typedef void (*PFN_ORTESleepMs)(unsigned int);

typedef char* (*PFN_IPAddressToString)(IPAddress, char *);

typedef IPAddress (*PFN_StringToIPAddress)(const char *);

typedef char* (*PFN_NtpTimeToStringMs)(NtpTime, char *);

typedef char* (*PFN_NtpTimeToStringUs)(NtpTime, char *);

#define __ORTE_API_ENTRY(symbol)	PFN_##symbol	pfn##symbol;
#define __ORTE_API_DECL(symbol)		__orte.apis.pfn##symbol

struct __ORTE_APIS {
	__ORTE_API_ENTRY(ORTEInit)
	__ORTE_API_ENTRY(ORTEDomainStart)
	__ORTE_API_ENTRY(ORTEDomainPropDefaultGet)
	__ORTE_API_ENTRY(ORTEDomainInitEvents)
	__ORTE_API_ENTRY(ORTEDomainAppCreate)
	__ORTE_API_ENTRY(ORTEDomainAppDestroy)
	__ORTE_API_ENTRY(ORTEDomainAppSubscriptionPatternAdd)
	__ORTE_API_ENTRY(ORTEDomainAppSubscriptionPatternRemove)
	__ORTE_API_ENTRY(ORTEDomainAppSubscriptionPatternDestroy)
	__ORTE_API_ENTRY(ORTEDomainMgrCreate)
	__ORTE_API_ENTRY(ORTEDomainMgrDestroy)
	__ORTE_API_ENTRY(ORTEPublicationCreate)
	__ORTE_API_ENTRY(ORTEPublicationDestroy)
	__ORTE_API_ENTRY(ORTEPublicationPropertiesGet)
	__ORTE_API_ENTRY(ORTEPublicationPropertiesSet)
	__ORTE_API_ENTRY(ORTEPublicationWaitForSubscriptions)
	__ORTE_API_ENTRY(ORTEPublicationGetStatus)
	__ORTE_API_ENTRY(ORTEPublicationSend)
	__ORTE_API_ENTRY(ORTEPublicationSendEx)
	__ORTE_API_ENTRY(ORTESubscriptionCreate)
	__ORTE_API_ENTRY(ORTESubscriptionDestroy)
	__ORTE_API_ENTRY(ORTESubscriptionPropertiesGet)
	__ORTE_API_ENTRY(ORTESubscriptionPropertiesSet)
	__ORTE_API_ENTRY(ORTESubscriptionWaitForPublications)
	__ORTE_API_ENTRY(ORTESubscriptionGetStatus)
	__ORTE_API_ENTRY(ORTESubscriptionPull)
	__ORTE_API_ENTRY(ORTETypeRegisterAdd)
	__ORTE_API_ENTRY(ORTETypeRegisterDestroyAll)
	__ORTE_API_ENTRY(ORTEVerbositySetOptions)
	__ORTE_API_ENTRY(ORTEVerbositySetLogFile)
	__ORTE_API_ENTRY(ORTESleepMs)
	__ORTE_API_ENTRY(IPAddressToString)
	__ORTE_API_ENTRY(StringToIPAddress)
	__ORTE_API_ENTRY(NtpTimeToStringMs)
	__ORTE_API_ENTRY(NtpTimeToStringUs)
};

struct __ORTE_DLL {
	HINSTANCE hInstance;
	DWORD dwResult;
	LONG cLoads;
	int fLoadTried;
	struct __ORTE_APIS apis;
};

extern struct __ORTE_DLL __orte;

DWORD __ORTEDllLoad(void);
DWORD __ORTEInit(void);

#define ORTEInit()				__ORTEInit()
#define ORTEDomainStart				__ORTE_API_DECL(ORTEDomainStart)
#define ORTEDomainPropDefaultGet		__ORTE_API_DECL(ORTEDomainPropDefaultGet)
#define ORTEDomainInitEvents			__ORTE_API_DECL(ORTEDomainInitEvents)
#define ORTEDomainAppCreate			__ORTE_API_DECL(ORTEDomainAppCreate)
#define ORTEDomainAppDestroy			__ORTE_API_DECL(ORTEDomainAppDestroy)
#define ORTEDomainAppSubscriptionPatternAdd	__ORTE_API_DECL(ORTEDomainAppSubscriptionPatternAdd)
#define ORTEDomainAppSubscriptionPatternRemove	__ORTE_API_DECL(ORTEDomainAppSubscriptionPatternRemove)
#define ORTEDomainAppSubscriptionPatternDestroy	__ORTE_API_DECL(ORTEDomainAppSubscriptionPatternDestroy)
#define ORTEDomainMgrCreate			__ORTE_API_DECL(ORTEDomainMgrCreate)
#define ORTEDomainMgrDestroy			__ORTE_API_DECL(ORTEDomainMgrDestroy)
#define ORTEPublicationCreate			__ORTE_API_DECL(ORTEPublicationCreate)
#define ORTEPublicationDestroy			__ORTE_API_DECL(ORTEPublicationDestroy)
#define ORTEPublicationPropertiesGet		__ORTE_API_DECL(ORTEPublicationPropertiesGet)
#define ORTEPublicationPropertiesSet		__ORTE_API_DECL(ORTEPublicationPropertiesSet)
#define ORTEPublicationWaitForSubscriptions	__ORTE_API_DECL(ORTEPublicationWaitForSubscriptions)
#define ORTEPublicationGetStatus		__ORTE_API_DECL(ORTEPublicationGetStatus)
#define ORTEPublicationSend			__ORTE_API_DECL(ORTEPublicationSend)
#define ORTEPublicationSendEx			__ORTE_API_DECL(ORTEPublicationSendEx)
#define ORTESubscriptionCreate			__ORTE_API_DECL(ORTESubscriptionCreate)
#define ORTESubscriptionDestroy			__ORTE_API_DECL(ORTESubscriptionDestroy)
#define ORTESubscriptionPropertiesGet		__ORTE_API_DECL(ORTESubscriptionPropertiesGet)
#define ORTESubscriptionPropertiesSet		__ORTE_API_DECL(ORTESubscriptionPropertiesSet)
#define ORTESubscriptionWaitForPublications	__ORTE_API_DECL(ORTESubscriptionWaitForPublications)
#define ORTESubscriptionGetStatus		__ORTE_API_DECL(ORTESubscriptionGetStatus)
#define ORTESubscriptionPull			__ORTE_API_DECL(ORTESubscriptionPull)
#define ORTETypeRegisterAdd			__ORTE_API_DECL(ORTETypeRegisterAdd)
#define ORTETypeRegisterDestroyAll		__ORTE_API_DECL(ORTETypeRegisterDestroyAll)
#define ORTEVerbositySetOptions			__ORTE_API_DECL(ORTEVerbositySetOptions)
#define ORTEVerbositySetLogFile			__ORTE_API_DECL(ORTEVerbositySetLogFile)
#define ORTESleepMs				__ORTE_API_DECL(ORTESleepMs)
#define IPAddressToString			__ORTE_API_DECL(IPAddressToString)
#define StringToIPAddress			__ORTE_API_DECL(StringToIPAddress)
#define NtpTimeToStringMs			__ORTE_API_DECL(NtpTimeToStringMs)
#define NtpTimeToStringUs			__ORTE_API_DECL(NtpTimeToStringUs)

#ifdef __cplusplus
	}
#endif