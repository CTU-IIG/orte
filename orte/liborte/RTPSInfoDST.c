/*
 *  $Id: RTPSInfoDST.c,v 0.0.0.1        2003/08/21
 *
 *  DEBUG:  section 42                  message INFO DST
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
void RTPSInfoDST(uint8_t *rtps_msg,MessageInterpret *mi) {
  HostId             hid;
  AppId              aid;

  //parsing
  hid=*((HostId*)(rtps_msg+4));                 /* HostId */
  conv_u32(&hid,0);
  aid=*((uint32_t*)(rtps_msg+8));              /* AppId */
  conv_u32(&aid,0);

  debug(42,3) ("  RTPSInfoDST:\n");
  debug(42,4) ("    hid:0x%x, aid:0x%x\n",hid,aid);

  if (hid!=HID_UNKNOWN) {
    mi->destHostId=hid;
  }
  if (aid!=AID_UNKNOWN) {
    mi->destAppId=aid;
  }
}


