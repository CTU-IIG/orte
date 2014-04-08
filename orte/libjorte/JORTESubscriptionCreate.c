/* JORTESubscriptionCreate.c  */

/**
  * This code provides conversion between JAVA a C environments.
  * The C functions are calling here and results are send to JAVA
  * native functions. It uses the header pregenerated by JAVA
  * (by command 'javah -jni class_with_native_function')
  *
  * @author Lukas Pokorny (lukas_pokorny@centrum.cz)
  * @author CTU FEE Prague - Department of Control Engineering (dce.felk.cvut.cz)
  * @author Project ORTE - OCERA Real Time Ethernet (www.ocera.org)
  * @author dedication to Kj
  * @version 0.1
  *
  *
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation; either version 2 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  * GNU General Public License for more details.
  *
  */


#include <stdlib.h>
#include <inttypes.h>

// library header file's path
#include "orte_all.h"
// pregenerated header
#include "jorte/org_ocera_orte_Subscription.h"
// enable TEST_STAGE run level
#include "jorte/4all.h"
// new data types
#include "jorte/jorte_typedefs_defines.h"
#include "jorte/jorte_protos_api.h"

/* ****************************************************************** *
 *                           CallBack function                        *
 * ****************************************************************** */

void
recvCallBack(const ORTERecvInfo *info,void *vinstance, void *recvCallBackParam)
{
  // jni varialbles
  JavaVM          *jvm = 0;
  JNIEnv          *env = 0;
  jclass           cls = 0; // local reference!
  jclass           cls_msg = 0;
  jobject          rinfo = 0;
  jobject          obj_msg;
  jobject          obj_bo = 0;
  jfieldID         fid = 0;
  jmethodID        mid = 0;
  jmethodID        mid_callback = 0;
  //
  // if the subscriber has been destroyed, return
  if((*(JORTECallbackContext_t**)recvCallBackParam) == 0)
    return;

  JORTECallbackContext_t   *callback_cont = *((JORTECallbackContext_t**)recvCallBackParam);

  #ifdef TEST_STAGE
    printf("\n\n:c: --------------- recvCallBack called.. --------------- \n");
  #endif

  do
  {
    // set local variables from struct
    if(callback_cont->jvm == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: jvm = NULL \n");
      #endif
      break;
    }
    jvm = callback_cont->jvm;
    // get env
    (*jvm)->AttachCurrentThread(jvm,
                                #ifdef __ANDROID__
                                  &env,
                                #else
                                  (void **)&env,
                                #endif
                                NULL);
    if(env == 0)
    {
      #ifdef TEST_STAGE
       printf(":!c: env = NULL \n");
      #endif
      break;
    }
    //
    // set byte order only if it differs from that currently set
    if(info->data_endian != callback_cont->cur_endian) {
      //prepare ByteOrder
      cls = (*env)->FindClass(env, "java/nio/ByteOrder");
      if (cls == 0) {
        #ifdef TEST_STAGE
          printf(":!c: cls = NULL \n");
        #endif
      }
      if(info->data_endian == BigEndian) {
        fid = (*env)->GetStaticFieldID(env,
                                       cls,
                                       "BIG_ENDIAN",
                                       "Ljava/nio/ByteOrder;");
        callback_cont->cur_endian = BigEndian;
      }
      else {
        fid = (*env)->GetStaticFieldID(env,
                                       cls,
                                       "LITTLE_ENDIAN",
                                       "Ljava/nio/ByteOrder;");
        callback_cont->cur_endian = LittleEndian;
      }
      if(fid == 0) {
        #ifdef TEST_STAGE
          printf(":!c: fid = NULL \n");
        #endif
      }
      obj_bo = (*env)->GetStaticObjectField(env, cls, fid);
      if(obj_bo == 0) {
        #ifdef TEST_STAGE
          printf(":!c: cls = NULL \n");
        #endif
      }

      // set byte order to ByteBuffer
      // get BB class
      cls = (*env)->GetObjectClass(env, callback_cont->obj_buf);
      if(cls == 0)
      {
        #ifdef TEST_STAGE
          printf(":!c: cls = NULL \n");
        #endif
      }
      // get methodID - order(ByteOrder)
      mid = (*env)->GetMethodID(env,
                                cls,
                                "order",
                                "(Ljava/nio/ByteOrder;)Ljava/nio/ByteBuffer;");
      if(mid == 0)
      {
        #ifdef TEST_STAGE
          printf(":!c: mid = NULL \n");
        #endif
      }

      // set ByteOrder
      if((*env)->CallObjectMethod(env,callback_cont->obj_buf,mid,obj_bo) == 0)
      {
        #ifdef TEST_STAGE
          printf(":!c: set byte order failed.. \n");
        #endif
      }
    }
    //
    if(callback_cont->obj == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: obj = NULL \n");
      #endif
      break;
    }
    // set local vars
    rinfo = callback_cont->rinfo;
    obj_msg = callback_cont->msg;


      #ifdef TEST_STAGE
         printf(":c: #0 \n");
         printf(":c: env = %#"PRIxPTR", obj_msg = %#"PRIxPTR" \n", (intptr_t)env, (intptr_t)obj_msg);
      #endif


	//
    if(rinfo == 0)
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
      rinfo = (*env)->NewObject(env, cls, mid);
      if(rinfo == 0)
      {
        #ifdef TEST_STAGE
          printf(":!c: rinfo = NULL \n");
        #endif
        break;
      }
      // create global reference
      callback_cont->rinfo = (*env)->NewGlobalRef(env, rinfo);
      if (callback_cont->rinfo == 0)
      {
        #ifdef TEST_STAGE
          printf(":!c: callback_cont->rinfo = NULL \n");
        #endif
        break;
      }
    }
    ////////////////////////////////////////////////////
    // set RecvInfo instance
    if(setRecvInfo(env,info,callback_cont->rinfo) == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: setRecvInfo() failed! \n");
      #endif
      break;
    }
    ////////////////////////////////////////////////////
    // control print - only in TEST_STAGE
    #ifdef TEST_STAGE
      printf(":c: rinfo created :] \n");
      printf(":c:----- ORTERecvInfo members  ------ \n");
      printf(":c:    recvStatus: %#x \n", info->status);
      printf(":c:    senderGuid: hid = %#"PRIx32", aid = %#"PRIx32", oid = %#"PRIx32" \n",
             info->senderGUID.hid,info->senderGUID.aid,info->senderGUID.oid);
      printf(":c:         topic: %s \n",info->topic);
      printf(":c:          type: %s \n",info->type);
      printf(":c: localTimeRecv: sec = %"PRId32", fract = %"PRIu32" \n",
             info->localTimeReceived.seconds,info->localTimeReceived.fraction);
      printf(":c: remoteTimePub: sec = %"PRId32", fract = %"PRIu32" \n",
             info->remoteTimePublished.seconds,info->remoteTimePublished.fraction);
      printf(":c:         seqNr: high = %"PRId32", low = %"PRIu32" \n",info->sn.high,info->sn.low);
      printf(":c:---------------------------------- \n");
    #endif
    ////////////////////////////////////////////////////
    // update MessageData instance
    // get cls
    cls_msg = (*env)->GetObjectClass(env, obj_msg);
    if(cls_msg == 0)
    {
      #ifdef TEST_STAGE
         printf(":!c: cls_msg = NULL \n");
      #endif
      break;
    }
    /////////////////////////////////////////////////////
    // methodID - read()
    mid = (*env)->GetMethodID(env,
                              cls_msg,
                              "read",
                              "()V");
    if(mid == 0)
    {
      #ifdef TEST_STAGE
         printf(":!c: mid = NULL \n");
      #endif
      break;
    }
    // call method
    (*env)->CallVoidMethod(env,
                           obj_msg,
                           mid);

   /* *************************** *
    *  call JAVA CallBack method  *
    * *************************** */
      #ifdef TEST_STAGE
        printf(":c: call JAVA CallBack method \n");
      #endif


	// get class
    cls = (*env)->GetObjectClass(env,callback_cont->obj);
    if(cls == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: cls = NULL \n");
      #endif
      break;
    }
    // get method ID
    mid = (*env)->GetMethodID(env,
                              cls,
                              "callback",
                              "(Lorg/ocera/orte/types/RecvInfo;Lorg/ocera/orte/types/MessageData;)V");
    if(mid == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: cls = NULL \n");
      #endif
      break;
    }
    mid_callback = mid;
    //
    #ifdef TEST_STAGE
      printf(":c: volam callback metodu.. halo jsi tam?? \n");
    #endif
    // call object's method
    (*env)->CallVoidMethod(env,
                           callback_cont->obj, /*obj*/
                           mid_callback,
                           callback_cont->rinfo,
                           obj_msg);
  } while(0);

  // detach current thread
  if((*jvm)->DetachCurrentThread(jvm) != 0)
     printf(":c!: DetachCurrentThread failed! \n");
  //
  #ifdef TEST_STAGE
     printf(":c: ------------ thats all from recvCallBack ------------ \n\n");
  #endif

}

/* ****************************************************************** *
 *                            native method                           *
 * ****************************************************************** */
JNIEXPORT jlong JNICALL
Java_org_ocera_orte_Subscription_jORTESubscriptionCreate
(JNIEnv   *env,
 jobject   obj,
 jlong     dhandle,   // appDomain handle
 jint      jsmode,    // subs mode
 jint      jstype,    // subs type
 jstring   jtopic,    // subs topic
 jstring   jtname,    // subs typeName
 jobject   jinstance, // direct ByteBuffer
 jint      jbyteOrder,// byte order of ByteBuffer
 jobject   obj_msg,   // messageData instance
 jobject   jdeadline,
 jobject   jminSeparation,
 jobject   obj_callback,
 jlong     j_multicastIP)
{
  // jni variables
  JavaVM                 *jvm;
  jfieldID                fid;
  jclass                  cls;
  // orte variables
  ORTESubscription       *s = 0;
  ORTEDomain             *d;
  SubscriptionMode        smode;
  SubscriptionType        stype;
  NtpTime                 deadline;
  NtpTime                 minSeparation;
  // jorte variable
  JORTECallbackContext_t *callback_cont;
  JORTECallbackContext_t **callback_cont_ptr;

  // standart variables
  const char             *topic = 0;
  const char             *typename = 0;
  void                   *buffer;
  int                     flag_ok = 0;
  // memory alocation
  // don't forget use free() funct.!!
  callback_cont = (JORTECallbackContext_t*)malloc(sizeof(JORTECallbackContext_t));
  callback_cont_ptr = (JORTECallbackContext_t**)malloc(sizeof(JORTECallbackContext_t*));
  *callback_cont_ptr = callback_cont;

  do
  {

    // get direct ByteBuffer pointer from Java
    buffer = (*env)->GetDirectBufferAddress(env, jinstance);
    // check obj_callback
    if (obj_callback == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: obj_callback = NULL \n");
      #endif
      break;
    }
    // get jvm
    jint b = (*env)->GetJavaVM(env,&jvm);
    if (b <  0)
    {
      #ifdef TEST_STAGE
        printf(":!c: getJavaVM() failed! \n");
      #endif
      break;
    }
    if (b == 0)
    {
      #ifdef TEST_STAGE
        printf(":c: getJavaVM succesfull.. \n");
      #endif
    }
    callback_cont->jvm = jvm;
    callback_cont->cur_endian = (CDR_Endianness) jbyteOrder;
    // create global references
    callback_cont->obj = (*env)->NewGlobalRef(env, obj_callback);
    //
    if (callback_cont->obj == 0)
    {
      #ifdef TEST_STAGE
        printf(":c: global reference not created! \n");
      #endif
      break;
    }
    // create global references
    callback_cont->obj_buf = (*env)->NewGlobalRef(env, jinstance);
    //
    if (callback_cont->obj_buf == 0)
    {
      #ifdef TEST_STAGE
        printf(":c: global reference not created! \n");
      #endif
      break;
    }
    // create global references
    callback_cont->msg = (*env)->NewGlobalRef(env, obj_msg);
    //
    if (callback_cont->msg == 0)
    {
      #ifdef TEST_STAGE
        printf(":c: global reference not created! \n");
      #endif
      break;
    }
    // init RecvInfo pointer
    callback_cont->rinfo = 0;
    //
    cls = (*env)->GetObjectClass(env, obj);
    if(cls == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: cls = NULL \n");
      #endif
      break;
    }
    // fieldID - callbackContextHandle
    fid = (*env)->GetFieldID(env,
                             cls,
                             "callbackContextHandle",
                             "J");
    if(fid == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: fid = NULL \n");
      #endif
      break;
    }
    (*env)->SetLongField(env,
                        obj,
                        fid,
                        (jlong) callback_cont_ptr);
    #ifdef TEST_STAGE
       printf(":c: ORTESubscriptionCreate() calling..\n");
    #endif
    //
    d = (ORTEDomain *) dhandle;
    if (d == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: d = NULL [bad domain handle] \n");
      #endif
      break;
    }
    //
    smode = (SubscriptionMode) jsmode;
    stype = (SubscriptionType) jstype;
    topic = (*env)->GetStringUTFChars(env, jtopic, 0);
    typename = (*env)->GetStringUTFChars(env, jtname, 0);
    deadline = getNtpTime(env, jdeadline);//
    minSeparation = getNtpTime(env, jminSeparation);//
    // call ORTE function
    s = ORTESubscriptionCreate(d,
                               smode,
                               stype,
                               topic,
                               typename,
                               buffer,
                               &deadline,
                               &minSeparation,
                               recvCallBack,
                               (void*)callback_cont_ptr,
                               (uint32_t) j_multicastIP);
    if (s == 0)
    {
      #ifdef TEST_STAGE
        printf(":!c: s = NULL [subscription not created] \n");
      #endif
      break;
    }

    // set flag
    flag_ok = 1;
  } while(0);

  // free memory
  (*env)->ReleaseStringUTFChars(env, jtopic, topic);
  (*env)->ReleaseStringUTFChars(env, jtname, typename);
  // returns handle of new created Subscription
  if(flag_ok == 0) return 0;
  return ((jlong) s);

}
