/*
 *  $Id: conv.c,v 0.0.0.1               2003/09/10
 *
 *  DEBUG:  section 7                 ordering conversion
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
 *  This module maintains a hash table of key/value pairs.
 *  Keys can be strings of any size, or numbers up to size
 *  unsigned long (HASHKEYTYPE).
 *  Values should be a pointer to some data you wish to store.
 *
 */

#include "orte.h"

/**********************************************************************************/
void conv_u16(u_int16_t *x,char ef) {
  #if WORDS_BIGENDIAN
     if(ef) *x=bswap_16(*x);
  #else
     if(!ef) *x=bswap_16(*x);
  #endif
}

/**********************************************************************************/
void conv_u32(u_int32_t *x,char ef) {
  #if WORDS_BIGENDIAN
     if(ef) *x=bswap_32(*x);
  #else
     if(!ef) *x=bswap_32(*x);
  #endif
}

/**********************************************************************************/
void conv_sn(SequenceNumber *sn,char ef) {
  #if WORDS_BIGENDIAN
     if(ef) {
       sn->high=bswap_32(sn->high);
       sn->low=bswap_32(sn->low);
     }  
  #else
     if(!ef) {
       sn->high=bswap_32(sn->high);
       sn->low=bswap_32(sn->low);
     }  
  #endif
}

/**********************************************************************************/
void conv_ntp(NtpTime *ntp,char ef) {
  #if WORDS_BIGENDIAN
     if(ef) {
       ntp->seconds=bswap_32(ntp->seconds);
       ntp->fraction=bswap_32(ntp->fraction);
     }  
  #else
     if(!ef) {
       ntp->seconds=bswap_32(ntp->seconds);
       ntp->fraction=bswap_32(ntp->fraction);
     }  
  #endif
}

/**********************************************************************************/
char* 
IPAddressToString(IPAddress ipAddress,char *buff) {
  struct in_addr addr;
  
  addr.s_addr=htonl(ipAddress);
  sprintf(buff,"%s",inet_ntoa(addr));
  return buff;
}

/**********************************************************************************/
IPAddress 
StringToIPAddress(const char *string) {
  return ntohl(inet_addr(string));
}

/**********************************************************************************/
char *
NtpTimeToStringMs(NtpTime time,char *buff) {
  int s,msec;
  
  NtpTimeDisAssembToMs(s, msec, time);
  sprintf(buff,"%d.%03d",s,msec);
  return buff;
}

/**********************************************************************************/
char *
NtpTimeToStringUs(NtpTime time,char *buff) {
  int s,usec;
  
  NtpTimeDisAssembToUs(s, usec, time);
  sprintf(buff,"%d.%06d",s,usec);
  return buff;
}
