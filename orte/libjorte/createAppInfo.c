/* createAppInfo.c  */

#include <jni.h>
#include "orte.h"
#include "jorte/4all.h"
#include "jorte/jorte_protos_api.h"

jobject createAppInfo(JNIEnv *env, ORTEAppInfo *ainfo)
{
  jclass    cls;
  jclass    cls_tmp;
  jobject   obj;
  jobject   obj_tmp;
  jfieldID  fid;
  jmethodID mid;
  //
  int flag_ok = 0;

  do
  {
    // find cls
    cls = (*env)->FindClass(env, "org/ocera/orte/types/AppInfo");
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
    // field ID - hostID
    fid = (*env)->GetFieldID(env,cls,"hostId","J");
    if(fid == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: fid = NULL \n");
      #endif
      break;
    }
    #ifdef TEST_STAGE
       printf(":c: ainfo.hostId = %d \n",ainfo->hostId);
    #endif
    (*env)->SetLongField(env,
                         obj,
                         fid,
                         (jlong) ainfo->hostId);

    // field ID - appID
    fid = (*env)->GetFieldID(env,cls,"appId","J");
    if(fid == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: fid = NULL \n");
      #endif
      break;
    }
    #ifdef TEST_STAGE
       printf(":c: ainfo.appId = %d \n",ainfo->appId);
    #endif
    (*env)->SetLongField(env,
                         obj,
                         fid,
                         (jlong) ainfo->appId);
    // field ID - unicastIPAddressTemp
    fid = (*env)->GetFieldID(env,cls,"unicastIPAddressTemp","J");
    if(fid == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: fid uniIPAddrTemp = NULL \n");
      #endif
      break;
    }
    #ifdef TEST_STAGE
       printf(":c: ainfo.unicastIPAddressList = %d \n",ainfo->unicastIPAddressList);
    #endif
    (*env)->SetLongField(env,
                         obj,
                         fid,
                         (jlong) *ainfo->unicastIPAddressList);
    // field ID - unicastIPcount
    fid = (*env)->GetFieldID(env,cls,"unicastIPcount","B");
    if(fid == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: fid = NULL \n");
      #endif
      break;
    }
    #ifdef TEST_STAGE
       printf(":c: ainfo.unicastIPcount = %d \n",
              ainfo->unicastIPAddressCount);
    #endif
    (*env)->SetByteField(env,
                         obj,
                         fid,
                         (jbyte) ainfo->unicastIPAddressCount);
    // field ID - multicastIPAddressTemp
    fid = (*env)->GetFieldID(env,cls,"mettaTrafficMulticastIPAddressTemp","J");
    if(fid == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: fid multiIPAddrTemp = NULL \n");
      #endif
      break;
    }
    #ifdef TEST_STAGE
       printf(":c: ainfo.multicastIPAddressList = %d \n",
              ainfo->metatrafficMulticastIPAddressList);
    #endif
    (*env)->SetLongField(env,
                         obj,
                         fid,
                         (jlong) *ainfo->metatrafficMulticastIPAddressList);
    // field ID - multicastIPcount
    fid = (*env)->GetFieldID(env,cls,"multicastIPcount","B");
    if(fid == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: fid = NULL \n");
      #endif
      break;
    }
    #ifdef TEST_STAGE
       printf(":c: ainfo.metatrafficMulticastIPAddressCount = %d \n",
              ainfo->metatrafficMulticastIPAddressCount);
    #endif
    (*env)->SetByteField(env,
                         obj,
                         fid,
                         (jbyte) ainfo->metatrafficMulticastIPAddressCount);

    // field ID - mettatraficUnicastPort
    fid = (*env)->GetFieldID(env,cls,"metatrafficUniPort","J");
    if(fid == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: fid = NULL \n");
      #endif
      break;
    }
    #ifdef TEST_STAGE
       printf(":c: ainfo.metatrafficUnicastPort = %d \n",
              ainfo->metatrafficUnicastPort);
    #endif
    (*env)->SetLongField(env,
                         obj,
                         fid,
                         (jlong) ainfo->metatrafficUnicastPort);

    // field ID - userdataUnicastPort
    fid = (*env)->GetFieldID(env,cls,"userdataUniPort","J");
    if(fid == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: fid = NULL \n");
      #endif
      break;
    }
    #ifdef TEST_STAGE
       printf(":c: ainfo.userdataUnicastPort = %d \n",
              ainfo->userdataUnicastPort);
    #endif
    (*env)->SetLongField(env,
                         obj,
                         fid,
                         (jlong) ainfo->userdataUnicastPort);

    // find cls - VendorID
    cls_tmp = (*env)->FindClass(env, "org/ocera/orte/types/VendorId");
    if(cls_tmp == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: cls_tmp = NULL \n");
      #endif
      break;
    }
    // call object constructor
    mid = (*env)->GetMethodID(env,
                              cls_tmp,
                              "<init>",
                              "(BB)V");
    if(mid == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: constructor failed! \n");
      #endif
      break;
    }
    // create new object
    obj_tmp = (*env)->NewObject(env,
                                cls_tmp,
                                mid,
                                ainfo->vendorId.major,
                                ainfo->vendorId.minor);
    if(obj_tmp == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: obj = NULL \n");
      #endif
      break;
    }
    // set AppInfo's field
    fid = (*env)->GetFieldID(env,
                             cls,
                             "vendorId",
                             "Lorg/ocera/orte/types/VendorId;");
    if(fid == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: fid = NULL \n");
      #endif
      break;
    }
    #ifdef TEST_STAGE
       printf(":c: ainfo.vendorId: major = %d, minor = %d \n",
              ainfo->vendorId.major, ainfo->vendorId.minor);
    #endif
    (*env)->SetObjectField(env,
                           obj,
                           fid,
                           obj_tmp);

    // find cls - VendorID
    cls_tmp = (*env)->FindClass(env, "org/ocera/orte/types/ProtocolVersion");
    if(cls_tmp == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: cls_tmp = NULL \n");
      #endif
      break;
    }
    // call object constructor
    mid = (*env)->GetMethodID(env,
                              cls_tmp,
                              "<init>",
                              "(BB)V");
    if(mid == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: constructor failed! \n");
      #endif
      break;
    }
    // create new object
    obj_tmp = (*env)->NewObject(env,
                                cls_tmp,
                                mid,
                                ainfo->protocolVersion.major,
                                ainfo->protocolVersion.minor);
    if(obj_tmp == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: obj = NULL \n");
      #endif
      break;
    }
    // set AppInfo's field
    fid = (*env)->GetFieldID(env,
                             cls,
                             "protocolVersion",
                             "Lorg/ocera/orte/types/ProtocolVersion;");
    if(fid == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: fid = NULL \n");
      #endif
      break;
    }
    #ifdef TEST_STAGE
       printf(":c: ainfo.protocolVersion: major = %d, minor = %d \n",
              ainfo->protocolVersion.major, ainfo->protocolVersion.minor);
    #endif
    (*env)->SetObjectField(env,
                           obj,
                           fid,
                           obj_tmp);

    flag_ok = 1;
  } while(0);

  if(!flag_ok)
  {
    //detach...()
    return NULL;
  }

return obj;
}
