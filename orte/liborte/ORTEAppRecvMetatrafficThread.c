/*
 *  $Id: ORTEAppRecvMetatrafficThread.c,v 0.0.0.1  2003/08/21 
 *
 *  DEBUG:  section 22                  Receiving thread
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

/*****************************************************************************/
void ORTEAppRecvMetatrafficThread(ORTEDomain *d) {
  struct sockaddr_in    des;
  int16_t               submsg_len,msg_ptr;
  u_int16_t             msg_total_len;
  MessageInterpret      mi; 

  debug(22,10) ("ORTEAppRecvMetatrafficThread: start\n");

  while (!d->taskRecvMetatraffic.terminate) {
    msg_total_len = sock_recvfrom(
                  &d->taskRecvMetatraffic.sock,  //socked handle
                  d->mbRecvMetatraffic.cdrStream.buffer, //buffer
                  d->domainProp.recvBuffSize,    //max length of message
                  &des,sizeof(des));              //info from sending host
    d->mbRecvMetatraffic.cdrStream.length=msg_total_len;
    d->mbRecvMetatraffic.cdrStream.bufferPtr=d->mbRecvMetatraffic.cdrStream.buffer;
    debug(22,7) ("ORTEAppRecvMetatrafficThread: fired\n");
    //is it header of valid RTPS packet?
    if (RTPSHeaderCheck(d->mbRecvMetatraffic.cdrStream.buffer,msg_total_len,&mi)==0) {
      debug(22,7) ("ORTEAppRecvMetatrafficThread: RTPS Heard OK\n");
      debug(22,7) ("  PV: %d.%d VID:%d.%d HID:0x%x AID:0x%x\n",
                    mi.sourceVersion.major,mi.sourceVersion.minor,
      mi.sourceVendorId.major,mi.sourceVendorId.minor,
      mi.sourceHostId,mi.sourceAppId);
      msg_ptr=16;
      do {
        // check if length of submessage header is OK 
        if ((msg_ptr+3)<=msg_total_len) {
          int8_t e_bit=d->mbRecvMetatraffic.cdrStream.buffer[msg_ptr+1] & 1;
          submsg_len=(u_int16_t)d->mbRecvMetatraffic.cdrStream.buffer[msg_ptr+2];
          conv_u16(&submsg_len,e_bit);
          // check if length of submessage OK 
          if ((submsg_len+msg_ptr+3)<=msg_total_len) {
            pthread_rwlock_wrlock(&d->objectEntry.objRootLock);    
            pthread_rwlock_wrlock(&d->objectEntry.htimRootLock);
            switch ((SubmessageId)d->mbRecvMetatraffic.cdrStream.buffer[msg_ptr]) {
              case VAR:
                RTPSVar(d,d->mbRecvMetatraffic.cdrStream.buffer+msg_ptr,&mi,ntohl(des.sin_addr.s_addr));
              break;
              case ACK:
                RTPSAck(d,d->mbRecvMetatraffic.cdrStream.buffer+msg_ptr,&mi,ntohl(des.sin_addr.s_addr));
              break;
              case HEARTBEAT:
                RTPSHeardBeat(d,d->mbRecvMetatraffic.cdrStream.buffer+msg_ptr,&mi);
              break;
              case GAP:
                RTPSGap(d,d->mbRecvMetatraffic.cdrStream.buffer+msg_ptr,&mi,ntohl(des.sin_addr.s_addr));
              break;
              case INFO_TS:
                RTPSInfoTS(d->mbRecvMetatraffic.cdrStream.buffer+msg_ptr,&mi);
              break;
              case INFO_SRC:
                RTPSInfoSRC(d->mbRecvMetatraffic.cdrStream.buffer+msg_ptr,&mi);
              break;
              case INFO_REPLY:
                RTPSInfoREPLY(d->mbRecvMetatraffic.cdrStream.buffer+msg_ptr,&mi);
              break;
              case INFO_DST:
                RTPSInfoDST(d->mbRecvMetatraffic.cdrStream.buffer+msg_ptr,&mi);
              break;
              case PAD:
                RTPSPad(d->mbRecvMetatraffic.cdrStream.buffer+msg_ptr,&mi);
              break;
              default:
              break;
            }  
            pthread_rwlock_unlock(&d->objectEntry.htimRootLock);
            pthread_rwlock_unlock(&d->objectEntry.objRootLock);    
            msg_ptr+=submsg_len+4;
          } else break;          /* submessage is too big */
        } else break;            /* submessage is too big */
      } while (msg_ptr<msg_total_len);
    }
  } 

  debug(22,10) ("ORTEAppRecvMetatrafficThread: finished\n");
  pthread_exit(NULL);
}
