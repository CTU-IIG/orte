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

#include "orte_all.h"

/**********************************************************************************/
void RTPSInfoSRC(CDR_Codec *cdrCodec,MessageInterpret *mi) {
  IPAddress          ipa;
  ProtocolVersion    protocol;
  VendorId           vid;
  HostId             hid;
  AppId              aid;
  char               sIPAddress[MAX_STRING_IPADDRESS_LENGTH];
  CDR_Endianness     data_endian;

  /* appIPAddress */
  CDR_get_ulong(cdrCodec,&ipa);
  
  /* ProtocolVersion */
  CDR_get_octet(cdrCodec,&protocol.major);
  CDR_get_octet(cdrCodec,&protocol.minor);

  /* Vendor Id */
  CDR_get_octet(cdrCodec,&vid.major);
  CDR_get_octet(cdrCodec,&vid.minor);

  /* next data are sent in big endianing */
  data_endian=cdrCodec->data_endian;
  cdrCodec->data_endian=FLAG_BIG_ENDIAN;

  /* HostId */
  CDR_get_ulong(cdrCodec,&hid);

  /* AppId */
  CDR_get_ulong(cdrCodec,&aid);

  cdrCodec->data_endian=data_endian;

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
