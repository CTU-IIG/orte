/* jorte_typedefs_api.h */

#include  <jni.h>
#include "orte.h"

extern int
setTopic(JNIEnv *env, jclass cls, jobject obj, const char *topic);

extern int
setType(JNIEnv *env, jclass cls, jobject obj, const char *type);

extern int
setRecvInfo(JNIEnv *env, const ORTERecvInfo *rinfo, jobject obj);

extern jobject
createAppInfo(JNIEnv *env, const ORTEAppInfo *ainfo);

extern jobject
createPubInfo(JNIEnv *env, const ORTEPubInfo *pinfo);

extern jobject
createSubInfo(JNIEnv *env, const ORTESubInfo *sinfo);

extern NtpTime
getNtpTime(JNIEnv *env, jobject obj);
