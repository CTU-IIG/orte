/* setRecvInfo.c  */

#include <jni.h>
#include <inttypes.h>
#include "orte.h"
#include "jorte/4all.h"
#include "jorte/jorte_protos_api.h"

int setRecvInfo(JNIEnv *env, const ORTERecvInfo *rinfo, jobject obj)
{
  jclass    cls;
  jclass    cls_tmp;
//  jobject   obj;
  jobject   obj_tmp;
  jfieldID  fid;
  jmethodID mid;
  //
  int flag_ok = 0;
  int32_t sec;
  uint32_t us;

  do
  {
    // find cls
    cls = findClass(env, "org.ocera.orte.types.RecvInfo");
    if(cls == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: cls = NULL \n");
      #endif
      break;
    }
    // field ID - status
    fid = (*env)->GetFieldID(env,cls,"status","B");
    if(fid == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: fid = NULL \n");
      #endif
      break;
    }
    #ifdef TEST_STAGE
       printf(":c: rinfo.status = %d \n",
              rinfo->status);
    #endif
    (*env)->SetByteField(env,
                         obj,
                         fid,
                         (jbyte) rinfo->status);
/////////////////////////////////////////////////
    // set topic
    if(!setTopic(env,cls,obj,rinfo->topic))
    {
      #ifdef TEST_STAGE
        printf(":!c: seTopic() failed! \n");
      #endif
      break;
    }
    // set type
    if(!setType(env,cls,obj,rinfo->type))
    {
      #ifdef TEST_STAGE
        printf(":!c: seType() failed! \n");
      #endif
      break;
    }
/////////////////////////////////////////////////
    // find cls - GUID_RTPS
    cls_tmp = findClass(env, "org.ocera.orte.types.GUID_RTPS");
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
                              "(JJJ)V");
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
                                (jlong)rinfo->senderGUID.hid,
                                (jlong)rinfo->senderGUID.aid,
                                (jlong)rinfo->senderGUID.oid);
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
                             "senderGUID",
                             "Lorg/ocera/orte/types/GUID_RTPS;");
    if(fid == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: fid = NULL \n");
      #endif
      break;
    }
    #ifdef TEST_STAGE
       printf(":c: rinfo.senderGUID: hid = %#"PRIx32", aid = %#"PRIx32", oid = %#"PRIx32" \n",
              rinfo->senderGUID.hid,rinfo->senderGUID.aid,rinfo->senderGUID.oid);
    #endif
    (*env)->SetObjectField(env,
                           obj,
                           fid,
                           obj_tmp);
/////////////////////////////////////////////////
    // find cls - NtpTime
    cls_tmp = findClass(env, "org.ocera.orte.types.NtpTime");
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
                              "(IJ)V");
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
                                (jint)rinfo->localTimeReceived.seconds,
                                (jlong)rinfo->localTimeReceived.fraction);
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
                             "localTimeRecv",
                             "Lorg/ocera/orte/types/NtpTime;");
    if(fid == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: fid = NULL \n");
      #endif
      break;
    }
    #ifdef TEST_STAGE
       NtpTimeDisAssembToUs(sec, us, rinfo->localTimeReceived);
       printf(":c: rinfo.NtpTime: %"PRId32".%"PRIu32" (sec = %"PRId32" fract = %"PRIu32") \n",
              sec, us,
              rinfo->localTimeReceived.seconds,rinfo->localTimeReceived.fraction);
    #endif
    (*env)->SetObjectField(env,
                           obj,
                           fid,
                           obj_tmp);
/////////////////////////////////////////////////
    // find cls - NtpTime
/*
    cls_tmp = (*env)->FindClass(env, "org/ocera/orte/types/NtpTime");
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
                              "(IJ)V");
    if(mid == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: constructor failed! \n");
      #endif
      break;
    }
*/
    // create new object
    obj_tmp = (*env)->NewObject(env,
                                cls_tmp,
                                mid,
                                (jint)rinfo->remoteTimePublished.seconds,
                                (jlong)rinfo->remoteTimePublished.fraction);
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
                             "remoteTimePub",
                             "Lorg/ocera/orte/types/NtpTime;");
    if(fid == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: fid = NULL \n");
      #endif
      break;
    }
    #ifdef TEST_STAGE
       NtpTimeDisAssembToUs(sec, us, rinfo->remoteTimePublished);
       printf(":c: rinfo.remoteTimePub: %"PRId32".%"PRIu32" (sec = %"PRId32" fract = %"PRIu32") \n",
              sec, us,
              rinfo->remoteTimePublished.seconds,rinfo->remoteTimePublished.fraction);
    #endif
    (*env)->SetObjectField(env,
                           obj,
                           fid,
                           obj_tmp);
/////////////////////////////////////////////////
    // find cls - SequenceNumber
    cls_tmp = findClass(env,"org.ocera.orte.types.SequenceNumber");
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
                              "(IJ)V");
    if(mid == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: constructor failed! # \n");
      #endif
      break;
    }
    // create new object
    obj_tmp = (*env)->NewObject(env,
                                cls_tmp,
                                mid,
                                (jint)rinfo->sn.high,
                                (jlong)rinfo->sn.low);
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
                             "sn",
                             "Lorg/ocera/orte/types/SequenceNumber;");
    if(fid == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: fid = NULL \n");
      #endif
      break;
    }
    #ifdef TEST_STAGE
       printf(":c: rinfo.sn: %"PRId64" (high = %"PRId32" low = %"PRId32") \n",
              (((int64_t)rinfo->sn.high << 32) + rinfo->sn.low),rinfo->sn.high,rinfo->sn.low);
    #endif
    (*env)->SetObjectField(env,
                           obj,
                           fid,
                           obj_tmp);
/////////////////////////////////////////////////

    flag_ok = 1;
  } while(0);

  if(!flag_ok)
  {
    //detach...()
    return 0;
  }

return 1;
}
