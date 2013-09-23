#include <stdlib.h>
// origin orte headers
#include "orte.h"
// pregenerated header
#include "jorte/org_ocera_orte_types_DomainEvents.h"
#include "jorte/4all.h"
#include "jorte/jorte_typedefs_defines.h"

JNIEXPORT jboolean JNICALL
Java_org_ocera_orte_types_DomainEvents_jORTEDomainEventsDestroy
(JNIEnv *env, jobject obj, jlong handle)
{
  free((void*)handle);

  #ifdef TEST_STAGE
     printf(":c: events destroyed.. \n");
  #endif

  return 1;
}
