/*
 *  $Id: defines_api.h,v 0.0.0.1        2003/08/21 
 *
 *  AUTHOR: Petr Smolik                 petr.smolik@wo.cz
 *
 *  ORTE - OCERA Real-Time Ethernet     http://www.ocera.org/
 *  --------------------------------------------------------------------
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 */

#ifndef _DEFINES_API_H
#define _DEFINES_API_H

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_INTERFACES            5

#define ORTE_DEFAULT_DOMAIN       0

/*  Sleep for Windows is a stupid loop with I/O :))) */
#ifdef _WIN32
/**
 * ORTESleepMs - suspends calling thread for given time
 * @x: time in miliseconds
 */
#include <windows.h>
#define ORTESleepMs(x)  Sleep(x)
#else
#if defined(_OMK_UNIX) || defined(HAVE_UNISTD_H)
  #include <unistd.h>
#endif
#define ORTESleepMs(x)  usleep(x*1000)
#endif

/*****************************************************************/
// Cmp two sequence numbers
// Return:  1 if sn1>sn2
//         -1 if sn1<sn2
//          0 if sn1=sn2
#define SeqNumberCmp(sn1, sn2) (                 \
          (sn1).high>(sn2).high ? 1 :            \
        ((sn1).high<(sn2).high ? -1 :            \
          ((sn1).low>(sn2).low ? 1 :             \
            ((sn1).low<(sn2).low ? -1 : 0)))     \
    )

/*****************************************************************/
#define SeqNumberInc(res,sn) {                   \
      (res) = (sn);                              \
          if (++(res).low==0) (res).high++;      \
        }


/*****************************************************************/
#define SeqNumberAdd(res,sn1,sn2) {              \
    (res).low = (sn1).low+(sn2).low;             \
    (res).high = (sn1).high+(sn2).high;          \
    if (((res).low < (sn1).low) ||               \
        ((res).low < (sn2).low)) {               \
      (res).high++;                              \
    }                                            \
}

/*****************************************************************/
#define SeqNumberDec(res,sn) {                   \
      (res) = (sn);                              \
          if ((res).low--==0) (res).high--;      \
        }

/*****************************************************************/
#define SeqNumberSub(res,sn1,sn2) {                  \
    (res).low = (sn1).low-(sn2).low;                 \
    (res).high = (sn1).high-(sn2).high;              \
    if ((res).low > (sn1).low) {                     \
      (res).high--;                                  \
    }                                                \
}

/*****************************************************************/
#define NtpTimeCmp(time1, time2)                     \
((((time1).seconds) > ((time2).seconds)) ? 1 :       \
 ((((time1).seconds) < ((time2).seconds)) ? -1 :     \
  ((((time1).fraction) > ((time2).fraction)) ? 1 :   \
   ((((time1).fraction) < ((time2).fraction)) ? -1 : 0))))


/*****************************************************************/
#define NtpTimeAdd(res, time1, time2) {              \
    (res).seconds  = (time1).seconds + (time2).seconds;   \
    (res).fraction = (time1).fraction + (time2).fraction; \
    if (((res).fraction < (time1).fraction) ||       \
        ((res).fraction < (time2).fraction)) {       \
      (res).seconds++;                               \
    }                                                \
}

/*****************************************************************/
#define NtpTimeSub(res, time1, time2) {              \
   (res).seconds  = (time1).seconds - (time2).seconds;   \
   (res).fraction = (time1).fraction - (time2).fraction; \
   if ((res).fraction > (time1).fraction) {          \
     (res).seconds--;                                \
   }                                                 \
}

/*****************************************************************/
/**
 * NtpTimeAssembFromMs - converts seconds and miliseconds to NtpTime
 * @time: time given in NtpTime structure
 * @s: seconds portion of given time
 * @msec: miliseconds portion of given time
 */
#define NtpTimeAssembFromMs(time, s, msec) {         \
    register u_int32_t ms = msec;                \
    (time).seconds  = s;                             \
    (time).fraction = (ms<<22) + ((ms*393)<<8);      \
}

/**
 * NtpTimeDisAssembToMs - converts NtpTime to seconds and miliseconds
 * @s: seconds portion of given time
 * @msec: miliseconds portion of given time
 * @time: time given in NtpTime structure
 */
#define NtpTimeDisAssembToMs(s, msec, time) {        \
    s    = (time).seconds;                           \
    msec = ((time).fraction - ((time).fraction>>6) - \
           ((time).fraction>>7) + (1<<21))>>22;      \
    if ((msec) >= 1000 ) { (msec) -= 1000; (s)++; }  \
}

/**
 * NtpTimeAssembFromUs - converts seconds and useconds to NtpTime
 * @time: time given in NtpTime structure
 * @s: seconds portion of given time
 * @usec: microseconds portion of given time
 */
#define NtpTimeAssembFromUs(time, s, usec) {         \
    register u_int32_t us = usec;                     \
    (time).seconds  = s;                             \
    (time).fraction = (us<<12)+ ((us*99)<<1)+ ((us*15 + ((us*61)>>7))>>4); \
}

/**
 * NtpTimeDisAssembToUs - converts NtpTime to seconds and useconds
 * @s: seconds portion of given time
 * @usec: microseconds portion of given time
 * @time: time given in NtpTime structure
 */
#define NtpTimeDisAssembToUs(s, usec, time) {        \
    register u_int32_t NtpTemp = (time).fraction;     \
    s    = (time).seconds;                           \
    usec = ((time).fraction - (NtpTemp>>5)-(NtpTemp>>7)-(NtpTemp>>8)- \
            (NtpTemp>>9)-(NtpTemp>>10) - (NtpTemp>>12) - \
            (NtpTemp>>13)-(NtpTemp>>14) + (1<<11)) >> 12; \
    if ((usec) >= 1000000) { (usec) -= 1000000; (s)++; } \
}

/**
 * Domain2Port - converts Domain value to IP Port value
 * @domain: 
 * @port:
 */
#define Domain2Port(d,p) {                   \
  p = RTPS_DEFAULT_PORT + d*10;              \
}

/**
 * Domain2PortMulticastUserdata - converts Domain value to userdata IP Port value
 * @domain: 
 * @port:
 */
#define Domain2PortMulticastUserdata(d,p) {  \
    p = RTPS_DEFAULT_PORT + d*10+1;          \
}

/**
 * Domain2PortMulticastMetatraffic - converts Domain value to metatraffic IP Port value
 * @domain: 
 * @port:
 */
#define Domain2PortMulticastMetatraffic(d,p) {        \
    p = RTPS_DEFAULT_PORT + d*10+2;          \
}

#ifdef __cplusplus
} /* extern "C"*/
#endif

#endif /* _DEFINES_API_H */
