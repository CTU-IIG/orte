/* AppInfo.java */

/**
 * Class AppInfo. 
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
/* TODO - IpAddr!! */
//import  java.net.Inet4Address;

/**
 * @author luky
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */

/* TODO - naucit se delat s poli JAVA/JNI */
public class AppInfo 
{
  private long            hostId;
  private long            appId;
//  private Inet4Address[]  unicastIPList; 
    private byte            unicastIPcount;	
//  private Inet4Address[]  multicastIPList; 
  private byte            multicastIPcount;
  private long            metatrafficUniPort;
  private long            userdataUniPort;
  private VendorId        vendorId;
  private ProtocolVersion protocolVersion;
	

  /* constructor */		
  public AppInfo()
  {
  
  }
   
  public long getHostId()
  {
  	return this.hostId;
  }

  public long getAppId()
  {
  	return this.appId;
  }

  public byte getUnicastIPcount()
  {
  	return this.unicastIPcount;
  }
  
  public byte getMulticastIPcount()
  {
  	return this.multicastIPcount;
  }
    
  public long getMetatrafficUniPort()
  {
  	return this.metatrafficUniPort;
  }

  public long getUserdataUniPort()
  {
  	return this.userdataUniPort;
  }

  public VendorId getVendorId()
  {
  	return this.vendorId;
  }

  public ProtocolVersion getProtocolVersion()
  {
  	return this.protocolVersion;
  }

}
