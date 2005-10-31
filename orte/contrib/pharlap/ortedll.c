#include "windows.h"
#include "ortedll.h"
#include <assert.h>

#ifdef _DEBUG
	#define ASSERT(x)		assert(x)
#else
	#define ASSERT(x)
#endif


#define COUNTOF(arr)		(sizeof(arr)/sizeof((arr)[0]))

#ifdef _DEBUG
	#define __ORTE_DLL_NAME		"orted.dll"
#else
	#define __ORTE_DLL_NAME		"orte.dll"
#endif

struct __ORTE_DLL __orte;

static const LPCSTR orteFnNames[] = {
	"ORTEInit",
	"ORTEDomainStart",
	"ORTEDomainPropDefaultGet",
	"ORTEDomainInitEvents",
	"ORTEDomainAppCreate",
	"ORTEDomainAppDestroy",
	"ORTEDomainAppSubscriptionPatternAdd",
	"ORTEDomainAppSubscriptionPatternRemove",
	"ORTEDomainAppSubscriptionPatternDestroy",
	"ORTEDomainMgrCreate",
	"ORTEDomainMgrDestroy",
	"ORTEPublicationCreate",
	"ORTEPublicationDestroy",
	"ORTEPublicationPropertiesGet",
	"ORTEPublicationPropertiesSet",
	"ORTEPublicationWaitForSubscriptions",
	"ORTEPublicationGetStatus",
	"ORTEPublicationSend",
	"ORTEPublicationSendEx",
	"ORTESubscriptionCreate",
	"ORTESubscriptionDestroy",
	"ORTESubscriptionPropertiesGet",
	"ORTESubscriptionPropertiesSet",
	"ORTESubscriptionWaitForPublications",
	"ORTESubscriptionGetStatus",
	"ORTESubscriptionPull",
	"ORTETypeRegisterAdd",
	"ORTETypeRegisterDestroyAll",
	"ORTEVerbositySetOptions",
	"ORTEVerbositySetLogFile",
	"ORTESleepMs",
	"IPAddressToString",
	"StringToIPAddress",
	"NtpTimeToStringMs",
	"NtpTimeToStringUs"
};

DWORD GetProcAddresses(
	HINSTANCE hInst,
	FARPROC *functionTable,
	const LPCSTR *names,
	unsigned count
)
{
	FARPROC *pfnMax = functionTable + count;

	while (functionTable < pfnMax) {
		*functionTable = GetProcAddress(hInst, *names);
		if (!*functionTable)
			return GetLastError();
		++functionTable;
		++names;
	}

	return ERROR_SUCCESS;
}

#if 0
#define ORTE_GET_PROC_ADDRESS(symbol)	__orte.apis.symbol = (PFN_##symbol)GetProcAddress(__orte.hInstance, #symbol)

int SomePointerNull(FARPROC *functionTable, unsigned count)
{
	FARPROC *max = functionTable + count;

	while (functionTable < max) {
		if (*functionTable == NULL)
			return 1;
		++functionTable;
	}

	return 0;
}
#endif

DWORD __ORTEDllLoad(void)
{
	if (!__orte.cLoads && !__orte.fLoadTried) {
		__orte.fLoadTried = 1;
		
		__orte.hInstance = LoadLibrary(__ORTE_DLL_NAME);
		
		if (__orte.hInstance) {
			
			ASSERT(sizeof(struct __ORTE_APIS) / sizeof(FARPROC) == COUNTOF(orteFnNames));

			__orte.dwResult = GetProcAddresses(
				__orte.hInstance,
				(FARPROC*)&__orte.apis,
				orteFnNames,
				COUNTOF(orteFnNames));
			
			if (__orte.dwResult != ERROR_SUCCESS) {
				FreeLibrary(__orte.hInstance);
				__orte.hInstance = NULL;
			} else {
				__orte.dwResult = ERROR_SUCCESS;
			}
		} else {
			__orte.dwResult = GetLastError();
		}
	}

	if (__orte.dwResult == ERROR_SUCCESS)
		InterlockedIncrement(&__orte.cLoads);

	return __orte.dwResult;
}

DWORD __ORTEInit(void)
{
	DWORD res = __ORTEDllLoad();

	if (res == ERROR_SUCCESS)
		__orte.apis.pfnORTEInit();

	return res;
}
