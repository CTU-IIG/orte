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
 int            value; // !! pro testovani
} JORTECallbackContext_t;


typedef struct
{
 jobject     obj_recvStatus;
 jobject     str_topic;
 jobject     str_type;
 jobject     obj_guid;
 jobject     obj_ntpT_loc;
 jobject     obj_ntpT_rem;
 jobject     obj_sn;
} RecvInfoVar_t;


