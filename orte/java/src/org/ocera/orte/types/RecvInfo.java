/* RecvInfo.java */
package org.ocera.orte.types;

/**
  * Class
  *
  * @author Lukas Pokorny (lukas_pokorny@centrum.cz)
  * @author CTU FEE Prague - Department of Control Engineering (dce.felk.cvut.cz)
  * @author Project ORTE - OCERA Real Time Ethernet (www.ocera.org)
  * @author dedication to Kj
  * @version 0.1
  *
  * typedef enum ORTESendStatus {
  *      NEED_DATA        = 0x01,
  *      CQL              = 0x02
  *    } ORTESendStatus;
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

public class RecvInfo
{
  public int i;

  private RecvStatus      status;
  private String          topic;
  private String          type;
  private GUID_RTPS       senderGUID;
  private NtpTime         localTimeRecv;
  private NtpTime         remoteTimePub;
  private SequenceNumber  sn;

  public RecvInfo()
  {
    i = 0;
	status        = new RecvStatus(0);
    topic         = new String(" ");
    type          = new String(" ");
    senderGUID    = new GUID_RTPS(0,0,0);
    localTimeRecv = new NtpTime(0);
    remoteTimePub = new NtpTime(0);
    sn            = new SequenceNumber(0,0);

	System.out.println(":j: instance of RecvInfo created..");
  }

// SET & GET

// RecvStatus
  // set status
  public RecvStatus getRecvStatus()
  {
    return status.get();
  }
  // set status
  public void setRecvStatus(int value)
  {
    status.set(value);
    return;
  }

// Topic
  public String getTopic()
  {
    return topic;
  }
  // set status
  public void setTopic(String t)
  {
    topic = t;
    return;
  }

// Type
  public String getType()
  {
    return type;
  }
  // set status
  public void setType(String t)
  {
    type = t;
    return;
  }


// SenderGuid
  // get GUID_RTPS
  public GUID_RTPS getSenderGuid()
  {
    return senderGUID.get();
  }
  // set GUID_RTPS
  public void setSenderGuid(long h, long a, long o)
  {
    senderGUID.set(h,a,o);
    return;
  }


// localTimeRecv
  // get ntpTime
  public NtpTime getLocalTimeRecv()
  {
    return localTimeRecv.get();
  }
  // set GUID_RTPS
  public void setLocalTimeRecv(int sec, long fraction)
  {
    localTimeRecv.set(sec,fraction);
    return;
  }


// remoteTimePub
  // get ntpTime
  public NtpTime getRemoteTimePub()
  {
    return remoteTimePub.get();
  }
  // set GUID_RTPS
  public void setRemoteTimePub(int sec, long fraction)
  {
    remoteTimePub.set(sec,fraction);
    return;
  }


// Sequence number
  // get sn
  public SequenceNumber getSeqNumber()
  {
    return sn.get();
  }
  // set sn
  public void setSeqNumber(int high, int low)
  {
    sn.set(high, low);
    return;
  }

}
