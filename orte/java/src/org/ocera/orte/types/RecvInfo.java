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


public class RecvInfo
{
  private byte            status;
  private String          topic;
  private String          typeName;
  private GUID_RTPS       senderGUID;
  private NtpTime         localTimeRecv;
  private NtpTime         remoteTimePub;
  private SequenceNumber  sn;

  public RecvInfo()
  {
	//System.out.println(":j: instance of RecvInfo created..");
  }


  public byte getRecvStatus()
  {
    return this.status;
  }

  public String getTopic()
  {
    return this.topic;
  }

  public String getTypeName()
  {
    return this.typeName;
  }

  public GUID_RTPS getSenderGuid()
  {
    return this.senderGUID.get();
  }

  public NtpTime getLocalTimeRecv()
  {
    return this.localTimeRecv.get();
  }

  public NtpTime getRemoteTimePub()
  {
    return this.remoteTimePub.get();
  }

  public SequenceNumber getSeqNumber()
  {
    return this.sn.get();
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
      sn = this.getSeqNumber();
      System.out.println(":j:         seqNr: " + sn.getDecimal());   	
  }
  
}
