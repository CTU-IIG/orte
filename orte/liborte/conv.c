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

#include "orte_all.h"

/**********************************************************************************/
//get part of string, div. by semi.
int
getStringPart(char *string,char divChar,int *iterator,char *buff) {
  char *cp;
  int len;
  
  if (!string || !buff) return -1;
  len=strlen(string);
  if (len<(*iterator)) return -2;
  cp=string+(*iterator);
  if (cp[0]!=0) {  //till is length>0
    char *dcp,tcp; 
    dcp=strchr(cp,divChar);
    if (!dcp) dcp=cp+strlen(cp);
    tcp=*dcp;         //save ending value
    *dcp=0;           //temporary end of string
    strcpy(buff,cp);  
    *dcp=tcp;         //restore value
    if (dcp[0]!=0) (*iterator)+=dcp-cp+1;//next value
    else (*iterator)=len;
    return 1;
  }
  return 0;
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
  IPAddress ipAddress=IPADDRESS_INVALID;
  
  ipAddress=ntohl(inet_addr(string));
#if defined HAVE_GETHOSTBYNAME
  {
    struct hostent *hostname; 
    if (ipAddress==0) {
      if ((hostname=gethostbyname(string))) {
        ipAddress=ntohl(*((long*)(hostname->h_addr_list[0])));
      }
    }
  }
#endif
  return ipAddress;
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
