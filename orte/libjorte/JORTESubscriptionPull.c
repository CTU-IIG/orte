#include "orte.h"
#include "jorte/4all.h"
#include "jorte/org_ocera_orte_Subscription.h"

JNIEXPORT jint JNICALL
Java_org_ocera_orte_Subscription_jORTESubscriptionPull
  (JNIEnv *env, jobject obj, jlong sub_handle)
{
  return ORTESubscriptionPull((ORTESubscription *)sub_handle);
}
