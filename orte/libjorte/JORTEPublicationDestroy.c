/* JORTEPublicationDestroy.c */

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
// origin orte headers
#include "orte.h"
// pregenerated header
#include "jorte/org_ocera_orte_Publication.h"
#include "jorte/4all.h"


JNIEXPORT jboolean JNICALL
Java_org_ocera_orte_Publication_jORTEPublicationDestroy
(JNIEnv *env, jclass cls, jlong pub_handle)
{
  int8_t    b;

  // call ORTE function
  b = ORTEPublicationDestroy((ORTEPublication *) pub_handle);
  if (b == ORTE_BAD_HANDLE)
  {
    printf(":!c: publication destroy failed! [bad publication handle] \n");
    return 1;
  }
  // free data buffer
  free(ORTEPublicationGetInstance((ORTEPublication *) pub_handle));
  #ifdef TEST_STAGE
    printf(":c: publication destroyed successfully.. \n");
  #endif
  return 1;
}

