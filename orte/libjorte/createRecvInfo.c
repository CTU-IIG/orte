/* createRecvInfo.c  */

#include <jni.h>
#include "orte.h"
#include "jorte/4all.h"
#include "jorte/jorte_protos_api.h"

jobject setRecvInfo(JNIEnv *env, const ORTERecvInfo *rinfo)
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
    cls = (*env)->FindClass(env, "org/ocera/orte/types/RecvInfo");
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
//ok
/*
  private byte            status;
  private String          topic;
  private String          typeName;
  private GUID_RTPS       senderGUID;
  private NtpTime         localTimeRecv;
  private NtpTime         remoteTimePub;
  private SequenceNumber  sn;
*/
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
    cls_tmp = (*env)->FindClass(env, "org/ocera/orte/types/GUID_RTPS");
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
       printf(":c: rinfo.senderGUID: hid = %d, aid = %d, oid = %d \n",
              rinfo->senderGUID.hid,rinfo->senderGUID.aid,rinfo->senderGUID.oid);
    #endif
    (*env)->SetObjectField(env,
                           obj,
                           fid,
                           obj_tmp);
/////////////////////////////////////////////////
    // find cls - NtpTime
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
       printf(":c: rinfo.NtpTime: %d (sec = %d fract = %d) \n",
              (rinfo->localTimeReceived.seconds + rinfo->localTimeReceived.fraction/2^32),
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
       printf(":c: rinfo.remoteTimePub: %d (sec = %d fract = %d) \n",
              (rinfo->remoteTimePublished.seconds + rinfo->remoteTimePublished.fraction/2^32),
              rinfo->remoteTimePublished.seconds,rinfo->remoteTimePublished.fraction);
    #endif
    (*env)->SetObjectField(env,
                           obj,
                           fid,
                           obj_tmp);
/////////////////////////////////////////////////
    // find cls - SequenceNumber
    cls_tmp = (*env)->FindClass(env,"org/ocera/orte/types/SequenceNumber");
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
       printf(":c: rinfo.sn: %d (high = %d low = %d) \n",
              (rinfo->sn.high*2^32 + rinfo->sn.low),rinfo->sn.high,rinfo->sn.low);
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
    return NULL;
  }

return obj;
}
