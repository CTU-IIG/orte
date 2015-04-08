/* RecvInfo.java */

/**
 * Class RecvInfo.
 *
 * @author Lukas Pokorny (lukas_pokorny@centrum.cz)
 * @author CTU FEE Prague - Department of Control Engineering (dce.felk.cvut.cz)
 * @author Project ORTE - OCERA Real Time Ethernet (www.ocera.org)
 * @author dedication to Kj
 * @version 0.1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

package org.ocera.orte.types;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class RecvInfo
{

  private ByteBuffer      info_buffer;
  
  /*      Helper buffer structure (all 32-bit integers):
   *       [0]  sizeof(ORTERecvInfo),
   *       [1]  sizeof(ORTERecvStatus),
   *       [2]  sizeof(char*),
   *       [3]  offsetof(ORTERecvInfo,status),
   *       [4]  offsetof(ORTERecvInfo,topic),
   *       [5]  offsetof(ORTERecvInfo,type),
   *       [6]  offsetof(ORTERecvInfo,senderGUID),
   *       [7]  offsetof(ORTERecvInfo,localTimeReceived),
   *       [8]  offsetof(ORTERecvInfo,remoteTimePublished),
   *       [9]  offsetof(ORTERecvInfo,sn),
   *     [10]  offsetof(GUID_RTPS,hid),
   *     [11]  offsetof(GUID_RTPS,aid),
   *     [12]  offsetof(GUID_RTPS,oid),
   *     [13]  offsetof(NtpTime,seconds),
   *     [14]  offsetof(NtpTime,fraction),
   *     [15]  offsetof(SequenceNumber,high),
   *     [16]  offsetof(SequenceNumber,low)
   */
  private ByteBuffer      helper_buffer;

  public RecvInfo()
  {
	//System.out.println(":j: instance of RecvInfo created..");
    this.helper_buffer = ByteBuffer.allocateDirect(17*Integer.SIZE/8);
    this.helper_buffer.order(ByteOrder.nativeOrder());
    this.c_helper(helper_buffer);

    this.info_buffer = ByteBuffer.allocateDirect(helper_buffer.getInt(0));
    this.info_buffer.order(ByteOrder.nativeOrder());
  }


  public byte getRecvStatus()
  {
    byte ret_val;
    
    switch(helper_buffer.getInt(Integer.SIZE/8)) {
      case 4:
        ret_val = (byte) info_buffer.getInt(helper_buffer.getInt(Integer.SIZE*3/8));
        break;
      case 1:
        ret_val = info_buffer.get(helper_buffer.getInt(Integer.SIZE*3/8));
        break;
      case 2:
        ret_val = (byte) info_buffer.getShort(helper_buffer.getInt(Integer.SIZE*3/8));
        break;
      case 8:
        ret_val = (byte) info_buffer.getLong(helper_buffer.getInt(Integer.SIZE*3/8));
        break;
    default:
        System.out.println("j: ORTERecvStatus of unknown size!");
        return -1;
    }

    return ret_val;
  }

  public String getTopic()
  {
    long topic_pointer;
    
    switch(helper_buffer.getInt(Integer.SIZE*2/8)) {
      case 4:
        topic_pointer = info_buffer.getInt(helper_buffer.getInt(Integer.SIZE*4/8));
        break;
      case 8:
        topic_pointer = info_buffer.getLong(helper_buffer.getInt(Integer.SIZE*4/8));
        break;
      case 1:
        topic_pointer = info_buffer.get(helper_buffer.getInt(Integer.SIZE*4/8));
        break;
      case 2:
        topic_pointer = info_buffer.getShort(helper_buffer.getInt(Integer.SIZE*4/8));
        break;
      default:
        return null;
    }
    
    return get_string(topic_pointer);
  }

  public String getTypeName()
  {
    long type_pointer;
    
    switch(helper_buffer.getInt(Integer.SIZE*2/8)) {
      case 4:
        type_pointer = info_buffer.getInt(helper_buffer.getInt(Integer.SIZE*5/8));
        break;
      case 8:
        type_pointer = info_buffer.getLong(helper_buffer.getInt(Integer.SIZE*5/8));
        break;
      case 1:
        type_pointer = info_buffer.get(helper_buffer.getInt(Integer.SIZE*5/8));
        break;
      case 2:
        type_pointer = info_buffer.getShort(helper_buffer.getInt(Integer.SIZE*5/8));
        break;
      default:
        return null;
    }
    
    return get_string(type_pointer);
  }

  public GUID_RTPS getSenderGuid()
  {
    long hostId   = info_buffer.getInt(helper_buffer.getInt(Integer.SIZE*6/8)+helper_buffer.getInt(Integer.SIZE*10/8)) & 0xffffffffL;
    long appId    = info_buffer.getInt(helper_buffer.getInt(Integer.SIZE*6/8)+helper_buffer.getInt(Integer.SIZE*11/8)) & 0xffffffffL;
    long objectId = info_buffer.getInt(helper_buffer.getInt(Integer.SIZE*6/8)+helper_buffer.getInt(Integer.SIZE*12/8)) & 0xffffffffL;
    
    
    return new GUID_RTPS(hostId, appId, objectId);
  }

  public NtpTime getLocalTimeRecv()
  {
    int  seconds  = info_buffer.getInt(helper_buffer.getInt(Integer.SIZE*7/8)+helper_buffer.getInt(Integer.SIZE*13/8));
    long fraction = info_buffer.getInt(helper_buffer.getInt(Integer.SIZE*7/8)+helper_buffer.getInt(Integer.SIZE*14/8)) & 0xffffffffL;

    return new NtpTime(seconds, fraction);
  }

  public NtpTime getRemoteTimePub()
  {
    int  seconds  = info_buffer.getInt(helper_buffer.getInt(Integer.SIZE*8/8)+helper_buffer.getInt(Integer.SIZE*13/8));
    long fraction = info_buffer.getInt(helper_buffer.getInt(Integer.SIZE*8/8)+helper_buffer.getInt(Integer.SIZE*14/8)) & 0xffffffffL;

    return new NtpTime(seconds, fraction);
  }

  public SequenceNumber getSeqNumber()
  {
    int high = info_buffer.getInt(helper_buffer.getInt(Integer.SIZE*9/8)+helper_buffer.getInt(Integer.SIZE*15/8));
    long low  = info_buffer.getInt(helper_buffer.getInt(Integer.SIZE*9/8)+helper_buffer.getInt(Integer.SIZE*16/8));

    return new SequenceNumber(high, low);
  }

  /** only for test purposes */
  public void print()
  {
	  System.out.println(":j: RecvInfo:");
	  System.out.println(":j:    recvStatus: " + this.getRecvStatus());
	  System.out.println(":j:         topic: " + this.getTopic()); 
	  System.out.println(":j:          type: " + this.getTypeName()); 
      System.out.println(":j:    senderGuid: " + this.getSenderGuid()); 
      System.out.println(":j: localTimerecv: " + this.getLocalTimeRecv()); 
      System.out.println(":j: remoteTimePub: " + this.getRemoteTimePub()); 
    System.out.println(":j:         seqNr: " + this.getSeqNumber().getDecimal());   	
  }
  
  public ByteBuffer getBuffer()
  {
    return this.info_buffer;
  }
  
  /* NATIVE FUNCTIONS */
  
  private native String get_string(long string_pointer);
  private native void c_helper(ByteBuffer buffer);
  
}
