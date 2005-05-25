/* createPubInfo.c  */

#include <jni.h>
#include "orte.h"
#include "jorte/4all.h"
#include "jorte/jorte_protos_api.h"

jobject createPubInfo(JNIEnv *env, ORTEPubInfo *pinfo)
{
  jclass    cls;
  jobject   obj;
  jmethodID mid;
  //
  int flag_ok = 0;

  do
  {
    // find cls
    cls = (*env)->FindClass(env, "org/ocera/orte/types/PubInfo");
    if(cls == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: cls = NULL \n");
      #endif
      break;
    }
    // call object constructor
    mid = (*env)->GetMethodID(env, cls, "<init>", "()V");
    if(mid == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: constructor failed! \n");
      #endif
      break;
    }
    // create new object
    obj = (*env)->NewObject(env, cls, mid);
    if(obj == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: obj = NULL \n");
      #endif
      break;
    }
/////////////////////////////////////////////////
    // set topic
    if(!setTopic(env,cls,obj,pinfo->topic))
    {
      #ifdef TEST_STAGE
        printf(":!c: seTopic() failed! \n");
      #endif
      break;
    }
    // set type
    if(!setType(env,cls,obj,pinfo->type))
    {
      #ifdef TEST_STAGE
        printf(":!c: seType() failed! \n");
      #endif
      break;
    }
/////////////////////////////////////////////////

    flag_ok = 1;
  } while(0);

  if(!flag_ok)
  {
    //detach...()
    return NULL;
  }

return obj;
}
