/*
 *  $Id: RTPSInfoTS.c,v 0.0.0.1         2003/09/10
 *
 *  DEBUG:  section 45                  message INFO TS
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
RTPSInfoTSCreate(u_int8_t *rtps_msg,u_int32_t max_msg_len,NtpTime time) {
  if (max_msg_len<12) return -1;
  rtps_msg[0]=(u_int8_t)INFO_TS;
  rtps_msg[1]=ORTE_MY_MBO; //I-bit = 0
  *((ParameterLength*)(rtps_msg+2))=8;
  *((NtpTime*)(rtps_msg+4))=time;
  return 0;
}

/**********************************************************************************/
void 
RTPSInfoTS(u_int8_t *rtps_msg,MessageInterpret *mi) {
  int8_t   e_bit=rtps_msg[1] & 0x01;
  int32_t  sec,msec;

  if ((rtps_msg[1] & 0x02)==0) {               /* I = bit */
    mi->haveTimestamp=ORTE_TRUE;
    mi->timestamp=*((NtpTime*)(rtps_msg+4));
    conv_sn((SequenceNumber*)&mi->timestamp,e_bit);
  } else {
    mi->haveTimestamp=ORTE_FALSE;
    NTPTIME_ZERO(mi->timestamp);
  }
  NtpTimeDisAssembToMs(sec,msec,mi->timestamp);
  if (mi->haveTimestamp)
    debug(45,3) ("recv: RTPSInfoTS, timestamp %lis %lims\n",sec,msec);
  else
    debug(45,3) ("recv: RTPSInfoTS\n");
}
