/*
 *  $Id: ORTEAppRecvUserdataThread.c,v 0.0.0.1  2003/08/21 
 *
 *  DEBUG:  section 23                  Receiving userdata thread
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
void ORTEAppRecvUserdataThread(ORTEDomain *d) {
  struct sockaddr_in    des;
  uint16_t              submsg_len;
  MessageInterpret      mi; 

  debug(23,10) ("ORTEAppRecvUserdataThread: start\n");

  while (!d->taskRecvUserdata.terminate) {
    debug(23,7) ("ORTEAppRecvUserdataThread: receiving\n");
    d->mbRecvUserdata.cdrStream.length = sock_recvfrom(
                  &d->taskRecvUserdata.sock,     //socked handle
                  d->mbRecvUserdata.cdrStream.buffer,//buffer
                  d->domainProp.recvBuffSize,    //max length of message
                  &des,sizeof(des));              //info from sending host
    d->mbRecvUserdata.cdrStream.bufferPtr=d->mbRecvUserdata.cdrStream.buffer;
    debug(23,7) ("ORTEAppRecvUserdataThread: fired\n");
    //is it header of valid RTPS packet?
    if (RTPSHeaderCheck(d->mbRecvUserdata.cdrStream.buffer,
                        d->mbRecvUserdata.cdrStream.length,&mi)==0) {
      debug(23,7) ("ORTEAppRecvUserdataThread: RTPS Heard OK\n");
      debug(23,7) ("  PV: %d.%d VID:%d.%d HID:0x%x AID:0x%x\n",
                    mi.sourceVersion.major,mi.sourceVersion.minor,
      mi.sourceVendorId.major,mi.sourceVendorId.minor,
      mi.sourceHostId,mi.sourceAppId);
      d->mbRecvUserdata.cdrStream.bufferPtr+=16;
      do {
        // check if length of submessage header is OK 
        if ((d->mbRecvUserdata.cdrStream.bufferPtr-
             d->mbRecvUserdata.cdrStream.buffer+3)<=d->mbRecvUserdata.cdrStream.length) {
          char e_bit=d->mbRecvUserdata.cdrStream.bufferPtr[1] & 1;
          submsg_len=*(uint16_t*)&d->mbRecvUserdata.cdrStream.bufferPtr[2];
          conv_u16(&submsg_len,e_bit);
          // check if length of submessage OK 
          if ((submsg_len+d->mbRecvUserdata.cdrStream.bufferPtr-
               d->mbRecvUserdata.cdrStream.buffer+3)<=d->mbRecvUserdata.cdrStream.length) {
            pthread_rwlock_wrlock(&d->objectEntry.objRootLock);    
            pthread_rwlock_wrlock(&d->objectEntry.htimRootLock);
            switch ((SubmessageId)d->mbRecvUserdata.cdrStream.bufferPtr[0]) {
              case VAR:
                //can't be
              break;
              case ACK:
                RTPSAck(d,d->mbRecvUserdata.cdrStream.bufferPtr,&mi,ntohl(des.sin_addr.s_addr));
              break;
              case HEARTBEAT:
                RTPSHeardBeat(d,d->mbRecvUserdata.cdrStream.bufferPtr,&mi);
              break;
              case GAP:
                //can't be
              break;
              case INFO_TS:
                RTPSInfoTS(d->mbRecvUserdata.cdrStream.bufferPtr,&mi);
              break;
              case INFO_SRC:
                RTPSInfoSRC(d->mbRecvUserdata.cdrStream.bufferPtr,&mi);
              break;
              case INFO_REPLY:
                RTPSInfoREPLY(d->mbRecvUserdata.cdrStream.bufferPtr,&mi);
              break;
              case INFO_DST:
                RTPSInfoDST(d->mbRecvUserdata.cdrStream.bufferPtr,&mi);
              break;
              case PAD:
                RTPSPad(d->mbRecvUserdata.cdrStream.bufferPtr,&mi);
              break;
              case ISSUE:
                RTPSIssue(d,d->mbRecvUserdata.cdrStream.bufferPtr,&mi,ntohl(des.sin_addr.s_addr));
              break;
              default:
                debug(23,5) ("ORTEAppRecvUserdataThread: unknown message :%d\n",d->mbRecvUserdata.cdrStream.bufferPtr[0]);
              break;
            }  
            pthread_rwlock_unlock(&d->objectEntry.htimRootLock);
            pthread_rwlock_unlock(&d->objectEntry.objRootLock);    
            d->mbRecvUserdata.cdrStream.bufferPtr+=submsg_len+4;
          } else break;          /* submessage is too big */
        } else break;            /* submessage is too big */
      } while ((d->mbRecvUserdata.cdrStream.bufferPtr-
                d->mbRecvUserdata.cdrStream.buffer)<d->mbRecvUserdata.cdrStream.length);
    }
  } 

  debug(23,10) ("ORTEAppRecvUserdataThread: finished\n");
  pthread_exit(NULL);
}
