/*
 *  $Id: RTPSInfoREPLY.c,v 0.0.0.1      2003/09/1
 *
 *  DEBUG:  section 43                  message INFO REPLY
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
int32_t 
RTPSInfoREPLYCreate(u_int8_t *rtps_msg,u_int32_t max_msg_len,
    IPAddress ipaddress,Port port) {

  if (max_msg_len<12) return -1;
  rtps_msg[0]=(u_int8_t)INFO_REPLY;
  rtps_msg[1]=ORTE_MY_MBO;
  *((ParameterLength*)(rtps_msg+2))=8;
  *((ObjectId*)(rtps_msg+4))=ipaddress;
  *((ObjectId*)(rtps_msg+8))=port;
  return 12;
} 

/**********************************************************************************/
void 
RTPSInfoREPLY(u_int8_t *rtps_msg,MessageInterpret *mi) {
  IPAddress          ipa;
  Port               port;
  int8_t             e_bit;

  //Parsing
  e_bit=rtps_msg[1] & 0x01;
  ipa=*((IPAddress*)(rtps_msg+4));              /* unicastReplyIPAddress */
  conv_u32(&ipa,e_bit);
  port=*((Port*)(rtps_msg+8));                  /* unicastReplyPort */
  conv_u32(&ipa,e_bit);
  
  debug(43,3) ("recv: RTPS InfoREPLY from 0x%x-0x%x\n",
                mi->sourceHostId,mi->sourceAppId);

  if (ipa!=IPADDRESS_INVALID) {
    mi->unicastReplyIPAddress=ipa;
  }
  mi->unicastReplyPort=port;
  if (rtps_msg[1] & 0x02) {
    ipa=*((u_int32_t*)(rtps_msg+12));           /* multicastReplyIPAddress */
    conv_u32(&ipa,e_bit);
    port=*((u_int32_t*)(rtps_msg+16));          /* multicastReplyPort */
    conv_u32(&port,e_bit);
    mi->multicastReplyIPAddress=ipa;
    mi->multicastReplyPort=port;
  } else {
    mi->multicastReplyIPAddress=IPADDRESS_INVALID;
    mi->multicastReplyPort     =PORT_INVALID;
  }
}

