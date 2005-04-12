/* setTopic.c */

#include <jni.h>
#include "jorte/4all.h"
#include "jorte/jorte_protos_api.h"

int setTopic(JNIEnv *env, jclass cls, jobject obj, const char *topic)
{
    jfieldID  fid;
    jstring   jstr;

    // field ID - topic
    fid = (*env)->GetFieldID(env,
                             cls,
                             "topic",
                             "Ljava/lang/String;");
    if(fid == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: topic - fid = NULL \n");
      #endif
      return 0;
    }
    #ifdef TEST_STAGE
       printf(":c: topic = %s \n",topic);
    #endif
   // create a new string
   jstr = (*env)->NewStringUTF(env, topic);
   if (jstr == 0)
   {
     #ifdef TEST_STAGE
       printf(":!c: topic - fid = NULL \n");
     #endif
     return 0;
   }
   // set field
   (*env)->SetObjectField(env, obj, fid, jstr);

   return 1;

}
