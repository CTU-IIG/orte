/*
 *  $Id: RTPSHeader.c,v 0.0.0.1  2003/08/21 
 *
 *  DEBUG:  section 40          Operations with header of RTPS
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

#include "orte.h"

/**********************************************************************************/
int16_t
RTPSHeaderCreate(u_int8_t *msg,HostId hid,AppId aid) {
  msg[0]='R';msg[1]='T'; msg[2]='P'; msg[3]='S';
  //Protocol version
  PROTOCOL_VERSION_1_0((*((ProtocolVersion*)(msg+4))));
  //Vendor id
  VENDOR_ID_UNKNOWN((*((VendorId*)(msg+6))));
  //HID
  conv_u32(&hid,0);
  *((HostId*)(msg+8))=hid;
  //AID
  conv_u32(&aid,0);
  *((AppId*)(msg+12))=aid;
  return 16;
} 
/**********************************************************************************/
int16_t
RTPSHeaderCheck(u_int8_t *msg,int32_t len,MessageInterpret *mi) {
  if (len<16) return -1;                          /* message is too small */
  if (msg[0]!='R') return -2;                     /* header is invalid */
  if (msg[1]!='T') return -2;                     /* header is invalid */
  if (msg[2]!='P') return -2;                     /* header is invalid */
  if (msg[3]!='S') return -2;                     /* header is invalid */
  mi->sourceVersion=*((ProtocolVersion*)(msg+4)); /* ProtocolVersion */
  mi->sourceVendorId=*((VendorId*)(msg+6));       /* Vendor Id */
  mi->sourceHostId=*((HostId*)(msg+8));           /* Host Id */
  conv_u32(&mi->sourceHostId,0);
  mi->sourceAppId=*((AppId*)(msg+12));            /* App Id */
  conv_u32(&mi->sourceAppId,0);
  mi->haveTimestamp=ORTE_FALSE;                   /* false */
  return 0;
}


