#include <stdlib.h>
#include <stdio.h>
#include <jni.h>
#include "orte.h"
#include "jorte/4all.h"
#include "jorte/jorte_typedefs_defines.h"
#include "jorte/jorte_protos_api.h"

// /////////////////////////////////////////////////////////////////////
//  public void onRegFail()
// /////////////////////////////////////////////////////////////////////
Boolean
onRegFail(void *param)
{
// ///////////////////////////////// SPOLECNA CAST //
  JavaVM          *jvm = NULL;
  JNIEnv          *env = NULL; // local reference
  jclass           cls;
  jmethodID        mid;
  //
  JORTEDomainEventsContext_t   *domain_events_cont = (JORTEDomainEventsContext_t*) param;
  int flag_ok = 0;

  #ifdef TEST_STAGE
      printf(":c: event func. 'onRegFail()' called.. \n");
  #endif

  do
  {
// ///////////////////////////////// SPOLECNA CAST //
    if(domain_events_cont->jvm == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: jvm = NULL! \n");
     #endif
     break;
    }
    jvm = domain_events_cont->jvm;
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
       printf(":!c: env = NULL! \n");
     #endif
     break;
    }
    // find class
    cls = findClass(env, "org.ocera.orte.types.DomainEvents");
    if(cls == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: cls = NULL! \n");
     #endif
     break;
    }
// ///////////////////////////////// SPOLECNA CAST //
    // get method ID
    mid = (*env)->GetMethodID(env,
                              cls,
                              "onRegFail",
                              "()V");
    if(mid == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: mid = NULL! \n");
     #endif
     break;
    }
    // call method
    (*env)->CallVoidMethod(env,
                           domain_events_cont->obj_de,
                           mid);
    flag_ok = 1;
  } while(0);
  // while broken
  if(flag_ok)
  {
    if((*jvm)->DetachCurrentThread(jvm) != 0)
    {
      printf(":c!: DetachCurrentThread fault! \n");
      return  ORTE_FALSE;
    }
  }

  return ORTE_TRUE;
}


// /////////////////////////////////////////////////////////////////////
//  Event - public void onMgrNew()
// /////////////////////////////////////////////////////////////////////
Boolean
onMgrNew(const struct ORTEAppInfo *appInfo, void *param)
{
// ///////////////////////////////// SPOLECNA CAST //
  JavaVM          *jvm = NULL;
  JNIEnv          *env = NULL; // local reference
  jclass           cls;
  jobject          obj_ainfo;
  jmethodID        mid;
  //
  JORTEDomainEventsContext_t   *domain_events_cont = (JORTEDomainEventsContext_t*) param;
  int flag_ok = 0;

  #ifdef TEST_STAGE
      printf(":c: event func. 'onMgrNew()' called.. \n");
  #endif

  do
  {
// ///////////////////////////////// SPOLECNA CAST //
    if(domain_events_cont->jvm == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: jvm = NULL! \n");
     #endif
     break;
    }
    jvm = domain_events_cont->jvm;
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
       printf(":!c: env = NULL! \n");
     #endif
     break;
    }
    // find class
    cls = findClass(env, "org.ocera.orte.types.DomainEvents");
    if(cls == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: cls = NULL! \n");
     #endif
     break;
    }
    // create AppInfo instance
    obj_ainfo = createAppInfo(env, appInfo);
    if(obj_ainfo == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: obj_ainfo = NULL! \n");
     #endif
     break;
    }
// ///////////////////////////////// SPOLECNA CAST //
    // get method ID
    mid = (*env)->GetMethodID(env,
                              cls,
                              "onMgrNew",
                              "(Lorg/ocera/orte/types/AppInfo;)V");
    if(mid == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: mid = NULL! \n");
     #endif
     break;
    }
    // call method
    (*env)->CallVoidMethod(env,
                           domain_events_cont->obj_de,
                           mid,
                           obj_ainfo);
    flag_ok = 1;
  } while(0);
  // while broken
  if(flag_ok)
  {
    if((*jvm)->DetachCurrentThread(jvm) != 0)
    {
      printf(":c!: DetachCurrentThread fault! \n");
      return  ORTE_FALSE;
    }
  }

  return ORTE_TRUE;
}


// /////////////////////////////////////////////////////////////////////
//  Event - public void onMgrDelete()
// /////////////////////////////////////////////////////////////////////
Boolean
onMgrDelete(const struct ORTEAppInfo *appInfo, void *param)
{
// ///////////////////////////////// SPOLECNA CAST //
  JavaVM          *jvm = NULL;
  JNIEnv          *env = NULL; // local reference
  jclass           cls;
  jobject          obj_ainfo;
  jmethodID        mid;
  //
  JORTEDomainEventsContext_t   *domain_events_cont = (JORTEDomainEventsContext_t*) param;
  int flag_ok = 0;

  #ifdef TEST_STAGE
      printf(":c: event func. 'onMgrDelete()' called.. \n");
  #endif

  do
  {
// ///////////////////////////////// SPOLECNA CAST //
    if(domain_events_cont->jvm == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: jvm = NULL! \n");
     #endif
     break;
    }
    jvm = domain_events_cont->jvm;
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
       printf(":!c: env = NULL! \n");
     #endif
     break;
    }
    // find class
    cls = findClass(env, "org.ocera.orte.types.DomainEvents");
    if(cls == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: cls = NULL! \n");
     #endif
     break;
    }
    // create AppInfo instance
    obj_ainfo = createAppInfo(env, appInfo);
    if(obj_ainfo == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: obj_ainfo = NULL! \n");
     #endif
     break;
    }
// ///////////////////////////////// SPOLECNA CAST //
    // get method ID
    mid = (*env)->GetMethodID(env,
                              cls,
                              "onMgrDelete",
                              "(Lorg/ocera/orte/types/AppInfo;)V");
    if(mid == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: mid = NULL! \n");
     #endif
     break;
    }
    // call method
    (*env)->CallVoidMethod(env,
                           domain_events_cont->obj_de,
                           mid,
                           obj_ainfo);
    flag_ok = 1;
  } while(0);
  // while broken
  if(flag_ok)
  {
    if((*jvm)->DetachCurrentThread(jvm) != 0)
    {
      printf(":c!: DetachCurrentThread fault! \n");
      return  ORTE_FALSE;
    }
  }

  return ORTE_TRUE;
}



// /////////////////////////////////////////////////////////////////////
//  Event - onAppRemoteNew
// /////////////////////////////////////////////////////////////////////
Boolean
onAppRemoteNew(const struct ORTEAppInfo *appInfo, void *param)
{
// ///////////////////////////////// SPOLECNA CAST //
  JavaVM          *jvm = NULL;
  JNIEnv          *env = NULL; // local reference
  jclass           cls;
  jobject          obj_ainfo;
  jmethodID        mid;
  //
  JORTEDomainEventsContext_t   *domain_events_cont = (JORTEDomainEventsContext_t*) param;
  int flag_ok = 0;

  #ifdef TEST_STAGE
      printf(":c: event func. 'onAppRemoteNew()' called.. \n");
  #endif

  do
  {
// ///////////////////////////////// SPOLECNA CAST //
    if(domain_events_cont->jvm == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: jvm = NULL! \n");
     #endif
     break;
    }
    jvm = domain_events_cont->jvm;
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
       printf(":!c: env = NULL! \n");
     #endif
     break;
    }
    // find class
    cls = findClass(env, "org.ocera.orte.types.DomainEvents");
    if(cls == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: cls = NULL! \n");
     #endif
     break;
    }
    // create AppInfo instance
    obj_ainfo = createAppInfo(env, appInfo);
    if(obj_ainfo == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: obj_ainfo = NULL! \n");
     #endif
     break;
    }
// ///////////////////////////////// SPOLECNA CAST //

    // get method ID
    mid = (*env)->GetMethodID(env,
                              cls,
                              "onAppRemoteNew",
                              "(Lorg/ocera/orte/types/AppInfo;)V");
    if(mid == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: mid = NULL! \n");
     #endif
     break;
    }
    // call method
    (*env)->CallVoidMethod(env,
                           domain_events_cont->obj_de,
                           mid,
                           obj_ainfo);
    flag_ok = 1;
  } while(0);
  // while broken
  if(flag_ok)
  {
    if((*jvm)->DetachCurrentThread(jvm) != 0)
    {
      printf(":c!: DetachCurrentThread fault! \n");
      return  ORTE_FALSE;
    }
  }

  return ORTE_TRUE;
}


// /////////////////////////////////////////////////////////////////////
//  public void onAppDelete() { }
// /////////////////////////////////////////////////////////////////////
Boolean
onAppDelete(const struct ORTEAppInfo *appInfo, void *param)
{
// ///////////////////////////////// SPOLECNA CAST //
  JavaVM          *jvm = NULL;
  JNIEnv          *env = NULL; // local reference
  jclass           cls;
  jobject          obj_ainfo;
  jmethodID        mid;
  //
  JORTEDomainEventsContext_t   *domain_events_cont = (JORTEDomainEventsContext_t*) param;
  int flag_ok = 0;

  #ifdef TEST_STAGE
      printf(":c: event func. 'onAppDelete()' called.. \n");
  #endif

  do
  {
// ///////////////////////////////// SPOLECNA CAST //
    if(domain_events_cont->jvm == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: jvm = NULL! \n");
     #endif
     break;
    }
    jvm = domain_events_cont->jvm;
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
       printf(":!c: env = NULL! \n");
     #endif
     break;
    }
    // find class
    cls = findClass(env, "org.ocera.orte.types.DomainEvents");
    if(cls == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: cls = NULL! \n");
     #endif
     break;
    }
    // create AppInfo instance
    obj_ainfo = createAppInfo(env, appInfo);
    if(obj_ainfo == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: obj_ainfo = NULL! \n");
     #endif
     break;
    }
// ///////////////////////////////// SPOLECNA CAST //
    // get method ID
    mid = (*env)->GetMethodID(env,
                              cls,
                              "onAppDelete",
                              "(Lorg/ocera/orte/types/AppInfo;)V");
    if(mid == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: mid = NULL! \n");
     #endif
     break;
    }
    // call method
    (*env)->CallVoidMethod(env,
                           domain_events_cont->obj_de,
                           mid,
                           obj_ainfo);
    flag_ok = 1;
  } while(0);
  // while broken
  if(flag_ok)
  {
    if((*jvm)->DetachCurrentThread(jvm) != 0)
    {
      printf(":c!: DetachCurrentThread fault! \n");
      return  ORTE_FALSE;
    }
  }

  return ORTE_TRUE;
}


// /////////////////////////////////////////////////////////////////////
//  public void onPubRemoteNew()
// /////////////////////////////////////////////////////////////////////
Boolean
onPubRemoteNew(const struct ORTEAppInfo *appInfo,
               const struct ORTEPubInfo *pubInfo,
               void *param)
{
// ///////////////////////////////// SPOLECNA CAST //
  JavaVM          *jvm = NULL;
  JNIEnv          *env = NULL; // local reference
  jclass           cls;
  jobject          obj_ainfo;
  jobject          obj_pinfo;
  jmethodID        mid;
  //
  JORTEDomainEventsContext_t   *domain_events_cont = (JORTEDomainEventsContext_t*) param;
  int flag_ok = 0;

  #ifdef TEST_STAGE
      printf(":c: event func. 'onPubRemoteNew()' called.. \n");
  #endif

  do
  {
// ///////////////////////////////// SPOLECNA CAST //
    if(domain_events_cont->jvm == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: jvm = NULL! \n");
     #endif
     break;
    }
    jvm = domain_events_cont->jvm;
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
       printf(":!c: env = NULL! \n");
     #endif
     break;
    }
    // find class
    cls = findClass(env, "org.ocera.orte.types.DomainEvents");
    if(cls == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: cls = NULL! \n");
     #endif
     break;
    }
    // create AppInfo instance
    obj_ainfo = createAppInfo(env, appInfo);
    if(obj_ainfo == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: obj_ainfo = NULL! \n");
     #endif
     break;
    }
    // create PubInfo instance
    obj_pinfo = createPubInfo(env, pubInfo);
    if(obj_pinfo == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: obj_pinfo = NULL! \n");
     #endif
     break;
    }
// ///////////////////////////////// SPOLECNA CAST //
    // get method ID
    mid = (*env)->GetMethodID(env,
                              cls,
                              "onPubRemoteNew",
                              "(Lorg/ocera/orte/types/AppInfo;Lorg/ocera/orte/types/PubInfo;)V");
    if(mid == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: mid = NULL! \n");
     #endif
     break;
    }
    // call method
    (*env)->CallVoidMethod(env,
                           domain_events_cont->obj_de,
                           mid,
                           obj_ainfo,
                           obj_pinfo);
    flag_ok = 1;
  } while(0);
  // while broken
  if(flag_ok)
  {
    if((*jvm)->DetachCurrentThread(jvm) != 0)
    {
      printf(":c!: DetachCurrentThread fault! \n");
      return  ORTE_FALSE;
    }
  }

  return ORTE_TRUE;
}


// /////////////////////////////////////////////////////////////////////
//public void onPubRemoteChanged()
// /////////////////////////////////////////////////////////////////////
Boolean
onPubRemoteChanged(const struct ORTEAppInfo *appInfo,
                   const struct ORTEPubInfo *pubInfo,
                   void *param)
{
// ///////////////////////////////// SPOLECNA CAST //
  JavaVM          *jvm = NULL;
  JNIEnv          *env = NULL; // local reference
  jclass           cls;
  jobject          obj_ainfo;
  jobject          obj_pinfo;
  jmethodID        mid;
  //
  JORTEDomainEventsContext_t   *domain_events_cont = (JORTEDomainEventsContext_t*) param;
  int flag_ok = 0;

  #ifdef TEST_STAGE
      printf(":c: event func. 'onPubRemoteChanged()' called.. \n");
  #endif

  do
  {
// ///////////////////////////////// SPOLECNA CAST //
    if(domain_events_cont->jvm == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: jvm = NULL! \n");
     #endif
     break;
    }
    jvm = domain_events_cont->jvm;
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
       printf(":!c: env = NULL! \n");
     #endif
     break;
    }
    // find class
    cls = findClass(env, "org.ocera.orte.types.DomainEvents");
    if(cls == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: cls = NULL! \n");
     #endif
     break;
    }
    // create AppInfo instance
    obj_ainfo = createAppInfo(env, appInfo);
    if(obj_ainfo == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: obj_ainfo = NULL! \n");
     #endif
     break;
    }
    // create PubInfo instance
    obj_pinfo = createPubInfo(env, pubInfo);
    if(obj_pinfo == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: obj_pinfo = NULL! \n");
     #endif
     break;
    }
// ///////////////////////////////// SPOLECNA CAST //
    // get method ID
    mid = (*env)->GetMethodID(env,
                              cls,
                              "onPubRemoteChanged",
                              "(Lorg/ocera/orte/types/AppInfo;Lorg/ocera/orte/types/PubInfo;)V");
    if(mid == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: mid = NULL! \n");
     #endif
     break;
    }
    // call method
    (*env)->CallVoidMethod(env,
                           domain_events_cont->obj_de,
                           mid,
                           obj_ainfo,
                           obj_pinfo);
    flag_ok = 1;
  } while(0);
  // while broken
  if(flag_ok)
  {
    if((*jvm)->DetachCurrentThread(jvm) != 0)
    {
      printf(":c!: DetachCurrentThread fault! \n");
      return  ORTE_FALSE;
    }
  }

  return ORTE_TRUE;
}




// /////////////////////////////////////////////////////////////////////
//  public void onPubDelete()
// /////////////////////////////////////////////////////////////////////
Boolean
onPubDelete(const struct ORTEAppInfo *appInfo,
            const struct ORTEPubInfo *pubInfo,
            void *param)
{
// ///////////////////////////////// SPOLECNA CAST //
  JavaVM          *jvm = NULL;
  JNIEnv          *env = NULL; // local reference
  jclass           cls;
  jobject          obj_ainfo;
  jobject          obj_pinfo;
  jmethodID        mid;
  //
  JORTEDomainEventsContext_t   *domain_events_cont = (JORTEDomainEventsContext_t*) param;
  int flag_ok = 0;

  #ifdef TEST_STAGE
      printf(":c: event func. 'onPubDelete()' called.. \n");
  #endif

  do
  {
// ///////////////////////////////// SPOLECNA CAST //
    if(domain_events_cont->jvm == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: jvm = NULL! \n");
     #endif
     break;
    }
    jvm = domain_events_cont->jvm;
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
       printf(":!c: env = NULL! \n");
     #endif
     break;
    }
    // find class
    cls = findClass(env, "org.ocera.orte.types.DomainEvents");
    if(cls == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: cls = NULL! \n");
     #endif
     break;
    }
    // create AppInfo instance
    obj_ainfo = createAppInfo(env, appInfo);
    if(obj_ainfo == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: obj_ainfo = NULL! \n");
     #endif
     break;
    }
    // create PubInfo instance
    obj_pinfo = createPubInfo(env, pubInfo);
    if(obj_pinfo == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: obj_pinfo = NULL! \n");
     #endif
     break;
    }
// ///////////////////////////////// SPOLECNA CAST //
    // get method ID
    mid = (*env)->GetMethodID(env,
                              cls,
                              "onPubDelete",
                              "(Lorg/ocera/orte/types/AppInfo;Lorg/ocera/orte/types/PubInfo;)V");
    if(mid == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: mid = NULL! \n");
     #endif
     break;
    }
    // call method
    (*env)->CallVoidMethod(env,
                           domain_events_cont->obj_de,
                           mid,
                           obj_ainfo,
                           obj_pinfo);
    flag_ok = 1;
  } while(0);
  // while broken
  if(flag_ok)
  {
    if((*jvm)->DetachCurrentThread(jvm) != 0)
    {
      printf(":c!: DetachCurrentThread fault! \n");
      return  ORTE_FALSE;
    }
  }

  return ORTE_TRUE;
}


// /////////////////////////////////////////////////////////////////////
//  Event - onSubRemoteNew
// /////////////////////////////////////////////////////////////////////
Boolean
onSubRemoteNew(const struct ORTEAppInfo *appInfo,
               const struct ORTESubInfo *subInfo,
               void *param)
{
// ///////////////////////////////// SPOLECNA CAST //
  JavaVM          *jvm = NULL;
  JNIEnv          *env = NULL; // local reference
  jobject          obj_ainfo;
  jobject          obj_sinfo;
  jclass           cls;
  jmethodID        mid;
  //
  JORTEDomainEventsContext_t   *domain_events_cont = (JORTEDomainEventsContext_t*) param;
  int flag_ok = 0;

  #ifdef TEST_STAGE
      printf(":c: event func. 'onSubRemoteNew()' called.. \n");
  #endif

  do
  {
// ///////////////////////////////// SPOLECNA CAST //
    if(domain_events_cont->jvm == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: jvm = NULL! \n");
     #endif
     break;
    }
    jvm = domain_events_cont->jvm;
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
       printf(":!c: env = NULL! \n");
     #endif
     break;
    }
    // find class
    cls = findClass(env, "org.ocera.orte.types.DomainEvents");
    if(cls == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: cls = NULL! \n");
     #endif
     break;
    }
    // create AppInfo instance
    obj_ainfo = createAppInfo(env, appInfo);
    if(obj_ainfo == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: obj_ainfo = NULL! \n");
     #endif
     break;
    }
    // create SubInfo instance
    obj_sinfo = createSubInfo(env, subInfo);
    if(obj_sinfo == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: obj_sinfo = NULL! \n");
     #endif
     break;
    }
// ///////////////////////////////// SPOLECNA CAST //
    // get method ID
    mid = (*env)->GetMethodID(env,
                              cls,
                              "onSubRemoteNew",
                              "(Lorg/ocera/orte/types/AppInfo;Lorg/ocera/orte/types/SubInfo;)V");
    if(mid == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: mid = NULL! \n");
     #endif
     break;
    }
    // call method
    (*env)->CallVoidMethod(env,
                           domain_events_cont->obj_de,
                           mid,
                           obj_ainfo,
                           obj_sinfo);
    flag_ok = 1;
  } while(0);
  // while broken
  if(flag_ok)
  {
    if((*jvm)->DetachCurrentThread(jvm) != 0)
    {
      printf(":c!: DetachCurrentThread fault! \n");
      return  ORTE_FALSE;
    }
  }

  return ORTE_TRUE;
}


// /////////////////////////////////////////////////////////////////////
//  public void onSubRemoteChanged() { }
// /////////////////////////////////////////////////////////////////////
Boolean
onSubRemoteChanged(const struct ORTEAppInfo *appInfo,
                   const struct ORTESubInfo *subInfo,
                   void *param)
{
// ///////////////////////////////// SPOLECNA CAST //
  JavaVM          *jvm = NULL;
  JNIEnv          *env = NULL; // local reference
  jclass           cls;
  jobject          obj_ainfo;
  jobject          obj_sinfo;
  jmethodID        mid;
  //
  JORTEDomainEventsContext_t   *domain_events_cont = (JORTEDomainEventsContext_t*) param;
  int flag_ok = 0;

  #ifdef TEST_STAGE
      printf(":c: event func. 'onSubRemoteChanged()' called.. \n");
  #endif

  do
  {
// ///////////////////////////////// SPOLECNA CAST //
    if(domain_events_cont->jvm == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: jvm = NULL! \n");
     #endif
     break;
    }
    jvm = domain_events_cont->jvm;
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
       printf(":!c: env = NULL! \n");
     #endif
     break;
    }
    // find class
    cls = findClass(env, "org.ocera.orte.types.DomainEvents");
    if(cls == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: cls = NULL! \n");
     #endif
     break;
    }
    // create AppInfo instance
    obj_ainfo = createAppInfo(env, appInfo);
    if(obj_ainfo == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: obj_ainfo = NULL! \n");
     #endif
     break;
    }
    // create SubInfo instance
    obj_sinfo = createSubInfo(env, subInfo);
    if(obj_sinfo == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: obj_sinfo = NULL! \n");
     #endif
     break;
    }
// ///////////////////////////////// SPOLECNA CAST //
    // get method ID
    mid = (*env)->GetMethodID(env,
                              cls,
                              "onSubRemoteChanged",
                              "(Lorg/ocera/orte/types/AppInfo;Lorg/ocera/orte/types/SubInfo;)V");
    if(mid == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: mid = NULL! \n");
     #endif
     break;
    }
    // call method
    (*env)->CallVoidMethod(env,
                           domain_events_cont->obj_de,
                           mid,
                           obj_ainfo,
                           obj_sinfo);
    flag_ok = 1;
  } while(0);
  // while broken
  if(flag_ok)
  {
    if((*jvm)->DetachCurrentThread(jvm) != 0)
    {
      printf(":c!: DetachCurrentThread fault! \n");
      return  ORTE_FALSE;
    }
  }

  return ORTE_TRUE;
}


// /////////////////////////////////////////////////////////////////////
//  public void onSubDelete() { }
// /////////////////////////////////////////////////////////////////////
Boolean
onSubDelete(const struct ORTEAppInfo *appInfo,
            const struct ORTESubInfo *subInfo,
            void *param)
{
// ///////////////////////////////// SPOLECNA CAST //
  JavaVM          *jvm = NULL;
  JNIEnv          *env = NULL; // local reference
  jclass           cls;
  jobject          obj_ainfo;
  jobject          obj_sinfo;
  jmethodID        mid;
  //
  JORTEDomainEventsContext_t   *domain_events_cont = (JORTEDomainEventsContext_t*) param;
  int flag_ok = 0;

  #ifdef TEST_STAGE
      printf(":c: event func. 'onSubDelete()' called.. \n");
  #endif

  do
  {
// ///////////////////////////////// SPOLECNA CAST //
    if(domain_events_cont->jvm == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: jvm = NULL! \n");
     #endif
     break;
    }
    jvm = domain_events_cont->jvm;
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
       printf(":!c: env = NULL! \n");
     #endif
     break;
    }
    // find class
    cls = findClass(env, "org.ocera.orte.types.DomainEvents");
    if(cls == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: cls = NULL! \n");
     #endif
     break;
    }
    // create AppInfo instance
    obj_ainfo = createAppInfo(env, appInfo);
    if(obj_ainfo == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: obj_ainfo = NULL! \n");
     #endif
     break;
    }
    // create SubInfo instance
    obj_sinfo = createSubInfo(env, subInfo);
    if(obj_sinfo == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: obj_sinfo = NULL! \n");
     #endif
     break;
    }
// ///////////////////////////////// SPOLECNA CAST //
    // get method ID
    mid = (*env)->GetMethodID(env,
                              cls,
                              "onSubDelete",
                              "(Lorg/ocera/orte/types/AppInfo;Lorg/ocera/orte/types/SubInfo;)V");
    if(mid == 0)
    {
     #ifdef TEST_STAGE
       printf(":!c: mid = NULL! \n");
     #endif
     break;
    }
    // call method
    (*env)->CallVoidMethod(env,
                           domain_events_cont->obj_de,
                           mid,
                           obj_ainfo,
                           obj_sinfo);
    flag_ok = 1;
  } while(0);
  // while broken
  if(flag_ok)
  {
    if((*jvm)->DetachCurrentThread(jvm) != 0)
    {
      printf(":c!: DetachCurrentThread fault! \n");
      return  ORTE_FALSE;
    }
  }

  return ORTE_TRUE;
}
