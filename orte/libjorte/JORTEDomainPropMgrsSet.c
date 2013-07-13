#include <string.h>
#include <stdlib.h>
#include "orte.h"
#include "jorte/4all.h"

#include "jorte/org_ocera_orte_types_DomainProp.h"

  JNIEXPORT jboolean JNICALL Java_org_ocera_orte_types_DomainProp_jORTEDomainPropMgrsSet
  (JNIEnv *env, jobject obj)
  {
    jclass         cls_dp = NULL;
    jfieldID       fid = NULL;
    jobject        obj_str = NULL;
    const char     *str = NULL;
    //
    int flag_ok = 0;

    ORTEDomainProp *dp;

	#ifdef TEST_STAGE
	  printf(":c: jORTEDomainPropMgrsSet() called.. \n");
	#endif

	do {
		cls_dp = (*env)->GetObjectClass(env, obj);

		if (cls_dp == 0) {
			printf(":!c: class 'org.ocera.orte.types.DomainProp' not found! \n");
			break;
		}

		fid = (*env)->GetFieldID(env,
				                 cls_dp,
				                 "handle",
				                 "J");
	    if(fid == 0)
	    {
	      #ifdef TEST_STAGE
	        printf(":!c: fid = NULL \n");
	      #endif
	      break;
	    }
	    dp = (ORTEDomainProp *) (*env)->GetLongField(env, obj, fid);
	    if(dp == 0)
	    {
	      #ifdef TEST_STAGE
	        printf(":!c: *dp = NULL \n");
	      #endif
	      break;
	    }

	    fid = (*env)->GetFieldID(env,
	    						 cls_dp,
	    						 "mgrs",
	    						 "Ljava/lang/String;");
	    if(fid == 0)
	    {
	      #ifdef TEST_STAGE
	        printf(":!c: fid = NULL \n");
	      #endif
	      break;
	    }

	    obj_str = (*env)->GetObjectField(env, obj, fid);
	    if(obj_str == 0)
	    {
	      #ifdef TEST_STAGE
	        printf(":!c: obj_str = NULL \n");
	      #endif
	      break;
	    }
	    str = (*env)->GetStringUTFChars(env,obj_str,0);
	    dp->mgrs=str;

	    flag_ok = 1;
	} while (0);

	if(flag_ok) {
	  return  ORTE_TRUE;
	}

	return ORTE_FALSE;
  }
