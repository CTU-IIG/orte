/* SendInfo.java */

/**
  * Class SendInfo
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

package org.ocera.orte.types;


public class SendInfo
{
  SendStatus      status;
  String          topic;
  String          type;
  GUID_RTPS       senderGUID;
  SequenceNumber  sn;

  // constructor
  public SendInfo()
  {
    // vytvorit objekt JSendStatus + nahrat z C
    // nahrat z C topic
    // nahrat z C type
    // vytvorit + nahrat GUID
    // vytvorit + nahrat SN
  }





}
