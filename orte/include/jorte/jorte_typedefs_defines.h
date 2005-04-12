/* typedefs_defines_jorte.h */

/**
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


# define UINT32__MIN 0
# define UINT32__MAX (2^32 -1)


typedef struct
{
 JavaVM        *jvm;
 jobject        obj;
 jobject        rinfo;
 jobject        msg;
 jobject        bbuff; // byteBuffer
} JORTECallbackContext_t;


typedef struct
{
 JavaVM        *jvm;
 jobject        obj_de;  // domain events
} JORTEDomainEventsContext_t;


typedef enum {
        ON_REG_FAIL       = 1,
        ON_MGR_NEW        = 2,
        ON_MGR_DELETE     = 3,
        ON_APP_REMOTE_NEW = 4,
        ON_APP_DELETE     = 5,
        ON_PUB_REMOTE     = 6,
        ON_PUB_DELETE     = 7,
        ON_SUB_REMOTE     = 8,
        ON_SUB_DELETE     = 9
      } JORTEDomainEvents_t;
