/* setType.c */

#include <jni.h>
#include "jorte/4all.h"
#include "jorte/jorte_protos_api.h"

int setType(JNIEnv *env, jclass cls, jobject obj, const char *typeName)
{
    jfieldID  fid;
    jstring   jstr;

    // field ID - type
    fid = (*env)->GetFieldID(env,
                             cls,
                             "typeName",
                             "Ljava/lang/String;");
    if(fid == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: type - fid = NULL \n");
      #endif
      return 0;
    }
    #ifdef TEST_STAGE
       printf(":c: type = %s \n",typeName);
    #endif
   // create a new string
   jstr = (*env)->NewStringUTF(env,typeName);
   if (jstr == 0)
   {
     #ifdef TEST_STAGE
       printf(":!c: type - fid = NULL \n");
     #endif
     return 0;
   }
   // set field
   (*env)->SetObjectField(env, obj, fid, jstr);

   return 1;

}
