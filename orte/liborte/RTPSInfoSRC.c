/*
 *  $Id: RTPSInfoSRC.c,v 0.0.0.1      2003/09/10
 *
 *  DEBUG:  section 44                  message INFO SRC
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
void RTPSInfoSRC(u_int8_t *rtps_msg,MessageInterpret *mi) {
  IPAddress          ipa;
  ProtocolVersion    protocol;
  VendorId           vid;
  HostId             hid;
  AppId              aid;
  int8_t             e_bit;
  char               sIPAddress[MAX_STRING_IPADDRESS_LENGTH];

  //Parsing
  e_bit=rtps_msg[1] & 0x01;
  ipa=*((IPAddress*)(rtps_msg+4));              /* appIPAddress */
  conv_u32(&ipa,e_bit);
  protocol=*((ProtocolVersion*)(rtps_msg+8));   /* ProtocolVersion */
  vid=*((VendorId*)(rtps_msg+10));              /* Vendor Id */
  hid=*((HostId*)(rtps_msg+12));                /* HostId */
  conv_u32(&hid,0);
  aid=*((AppId*)(rtps_msg+16));                 /* AppId */
  conv_u32(&aid,0);

  debug(44,3) ("  RTPSInfoSRC: \n");
  debug(44,4) ("    appIPAddress:%s\n",IPAddressToString(ipa,sIPAddress));
  debug(44,4) ("    pv:%lu,%lu vid:%lu,%lu hid:0x%x aid:0x%x\n",
                             protocol.major,protocol.minor,vid.major,vid.minor,hid,aid);

  mi->sourceHostId=hid;
  mi->sourceAppId=aid;
  mi->sourceVersion=protocol;
  mi->sourceVendorId=vid;
  mi->unicastReplyIPAddress=ipa;
  mi->unicastReplyPort=PORT_INVALID;
  mi->multicastReplyIPAddress=IPADDRESS_INVALID;
  mi->multicastReplyPort=PORT_INVALID;
  mi->haveTimestamp=ORTE_FALSE;
}
