#ifndef IncludedOnLoad
#define IncludedOnLoad

#include <jni.h>

jclass findClass(JNIEnv *env, const char* name);

#endif
