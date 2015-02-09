/* 4all.h */
/*
typedef struct
{
 JavaVM        *jvm;
 jobject        obj;
 jobject        rinfo;
 int            value; // !! pro testovani
} JORTECallbackContext_t;
*/
// pro test purposes only
#ifndef Included4all
#define Included4all

//#define TEST_STAGE

#ifdef __ANDROID__

#include <android/log.h>
#define printf(...)             __android_log_print(ANDROID_LOG_INFO, "ORTE", __VA_ARGS__)

#endif
#endif
