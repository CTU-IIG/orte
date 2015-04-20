/* jorte_typedefs_api.h */

#include  <jni.h>
#include "orte.h"

extern int
setTopic(JNIEnv *env, jclass cls, jobject obj, const char *topic);

extern int
setType(JNIEnv *env, jclass cls, jobject obj, const char *type);

extern jobject
createAppInfo(JNIEnv *env, const ORTEAppInfo *ainfo);

extern jobject
createPubInfo(JNIEnv *env, const ORTEPubInfo *pinfo);

extern jobject
createSubInfo(JNIEnv *env, const ORTESubInfo *sinfo);

extern NtpTime
getNtpTime(JNIEnv *env, jobject obj);

extern Boolean
onSubDelete(const struct ORTEAppInfo *appInfo, const struct ORTESubInfo *subInfo, void *param);

extern Boolean
onSubRemoteChanged(const struct ORTEAppInfo *appInfo, const struct ORTESubInfo *subInfo, void *param);

extern Boolean
onSubRemoteNew(const struct ORTEAppInfo *appInfo, const struct ORTESubInfo *subInfo, void *param);

extern Boolean
onPubDelete(const struct ORTEAppInfo *appInfo, const struct ORTEPubInfo *pubInfo, void *param);

extern Boolean
onPubRemoteChanged(const struct ORTEAppInfo *appInfo, const struct ORTEPubInfo *pubInfo, void *param);

extern Boolean
onPubRemoteNew(const struct ORTEAppInfo *appInfo, const struct ORTEPubInfo *pubInfo, void *param);

extern Boolean
onAppDelete(const struct ORTEAppInfo *appInfo, void *param);

extern Boolean
onAppRemoteNew(const struct ORTEAppInfo *appInfo, void *param);

extern Boolean
onMgrDelete(const struct ORTEAppInfo *appInfo, void *param);

extern Boolean
onMgrNew(const struct ORTEAppInfo *appInfo, void *param);

extern Boolean
onRegFail(void *param);

extern jclass
findClass(JNIEnv *env, const char *name);
