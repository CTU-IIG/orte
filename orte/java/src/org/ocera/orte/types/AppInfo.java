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
import  java.net.Inet4Address;

/**
 * @author luky
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */

/* TODO - naucit se delat s poli JAVA/JNI */
public class AppInfo 
{
/*  
struct ORTEAppInfo {
HostId hostId;
AppId appId;

IPAddress * unicastIPAddressList;
unsigned char unicastIPAddressCount;

IPAddress * metatrafficMulticastIPAddressList;
unsigned char metatrafficMulticastIPAddressCount;

Port metatrafficUnicastPort;
Port userdataUnicastPort;

VendorId vendorId;
ProtocolVersion protocolVersion;
};
*/
    
  private long            hostId;
  private long            appId;
  /* TODO make an array of InetAddress[] instances */
  // temporaly 1 address not list
  private long            unicastIPAddressTemp;  
  private byte            unicastIPcount;	
  /* TODO make an array of InetAddress[] instances */
  // temporaly 1 address not list
  private long            mettaTrafficMulticastIPAddressTemp;	  
  private byte            multicastIPcount;
  private long            metatrafficUniPort;
  private long            userdataUniPort;
  private VendorId        vendorId;
  private ProtocolVersion protocolVersion;
  	
  private Inet4Address ip;  
  /* constructor */		
  public AppInfo()
  {
  
  }

  // ONLY FOR TESTING PURPOSES  
  public void setHostId(long hostId)
  {
      this.hostId = hostId;	
      return;
  }  
  // ONLY FOR TESTING PURPOSES  
  public void setAppId(long appId)
  {
      this.appId = appId;	
      return;
  }    
  // ONLY FOR TESTING PURPOSES  
  
  public long getHostId()
  {
  	return this.hostId;
  }

  public long getAppId()
  {
  	return this.appId;
  }

  public long getIpAddressTemp()
  {
  	return this.unicastIPAddressTemp;
  }
    
  public byte getUnicastIPcount()
  {
  	return this.unicastIPcount;
  }
  
  public byte getMulticastIPcount()
  {
  	return this.multicastIPcount;
  }
    
  public long getMettaTrafficMulticastIPAddressTemp() 
  {
        return this.mettaTrafficMulticastIPAddressTemp;
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

    
  // only for testing purposes
  public void printWithLegend()
  {
  	System.out.println(":j: * AppInfo.hostId = " + getHostId());
	System.out.println(":j: * AppInfo.appId  = " + getAppId());
	// ipAdressList
	System.out.println(":j: * AppInfo.pAddressTemp  = " + getIpAddressTemp());
	System.out.println(":j: * AppInfo.unicastIPcount = " + getUnicastIPcount());
	// ipAdressList
	System.out.println(":j: * AppInfo.pAddressTemp  = " + getIpAddressTemp());
        System.out.println(":j: * AppInfo.unicastIPcount = " + getMulticastIPcount());
	System.out.println(":j: * AppInfo.mettatrafficUniPort = " + getMetatrafficUniPort());
	System.out.println(":j: * AppInfo.userdataUniPort     = " + getUserdataUniPort());
	System.out.println(":j: * AppInfo.vendorId = " + getVendorId());
	System.out.println(":j: * AppInfo.protocolVersion = " + getProtocolVersion()); 	
  }
  
}
