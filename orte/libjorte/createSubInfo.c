/* createSubInfo.c  */

#include <jni.h>
#include "orte.h"
#include "jorte/4all.h"
#include "jorte/jorte_protos_api.h"

jobject createSubInfo(JNIEnv *env, ORTESubInfo *sinfo)
{
  jclass    cls;
  jobject   obj;
  jmethodID mid;

  //
  int flag_ok = 0;

  do
  {
    // find cls
    cls = (*env)->FindClass(env, "org/ocera/orte/types/SubInfo");
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
    if(!setTopic(env,cls,obj,sinfo->topic))
    {
      #ifdef TEST_STAGE
        printf(":!c: seTopic() failed! \n");
      #endif
      break;
    }
    // set type
    if(!setType(env,cls,obj,sinfo->type))
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
