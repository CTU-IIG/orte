#include <string.h>
#include <stdlib.h>
#include "orte.h"
#include "jorte/4all.h"

#include "jorte/org_ocera_orte_types_DomainProp.h"

  JNIEXPORT jboolean JNICALL Java_org_ocera_orte_types_DomainProp_jORTEDomainPropSet
  (
   JNIEnv *env,
   jobject obj,
   jlong propshandle,        //ORTEDomainProp handle
   jstring mgrs              //Managers
  )
  {
    const char     *str = NULL;
    //
    int flag_ok = 0;

    ORTEDomainProp *dp = NULL;

    #ifdef TEST_STAGE
      printf(":c: jORTEDomainPropSet() called.. \n");
    #endif

    do {
      dp = (ORTEDomainProp *) propshandle;
      if(dp == 0) {
        #ifdef TEST_STAGE
          printf(":!c: *dp = NULL \n");
        #endif
        break;
      }

      //call ReleaseStringUTFChars from destructor !!
      str = (*env)->GetStringUTFChars(env,mgrs,0);
      
      //set managers
      dp->mgrs=str;

      flag_ok = 1;
    } while (0);

    if(flag_ok) {
      return  ORTE_TRUE;
    }

    return ORTE_FALSE;
  }