#include <jni.h>

#ifndef IncludedOnLoad
#define IncludedOnLoad
#ifdef __cplusplus
extern "C" {
#endif

jclass findClass(JNIEnv *env, const char* name);

#ifdef __cplusplus
}
#endif
#endif
