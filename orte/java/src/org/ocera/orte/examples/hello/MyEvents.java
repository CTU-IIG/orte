/* MyEvents.java */

/**
 * Class MyEvents 
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

import org.ocera.orte.types.*;

public class MyEvents extends DomainEvents {

  /* constructor */
  public MyEvents()
  {
    super();	  	
  }

  public void onRegFail() 
  {
  	System.out.println(":j: *************************************************");
  	System.out.println(":j: * call method 'MyEvents.onRegFail()'..          *");
  	System.out.println(":j: *************************************************");
  }

  public void onMgrNew(AppInfo appInfo) 
  {
  	System.out.println(":j: *************************************************");
  	System.out.println(":j: * call method 'MyEvents.onMgrNew()'..           *");
  	System.out.println(":j: *************************************************");
  }
  
  public void onMgrDelete(AppInfo appInfo)
  {
  	System.out.println(":j: *************************************************");
  	System.out.println(":j: * call method 'MyEvents.onMgrDelete()'..        *");
  	System.out.println(":j: *************************************************");
  }

  public void onAppRemoteNew(AppInfo appInfo)
  {
  	System.out.println(":j: *************************************************");
  	System.out.println(":j: * call method 'MyEvents.onAppRemoteNew()'..     *");
  	System.out.println(":j: *************************************************");
    //
  	appInfo.printWithLegend();
  }

  public void onAppDelete(AppInfo appInfo)
  {
  	System.out.println(":j: *************************************************");
  	System.out.println(":j: * call method 'MyEvents.onAppDelete()'..        *");
  	System.out.println(":j: *************************************************");
    //
    appInfo.printWithLegend();   
  }

  public void onPubRemoteNew(AppInfo appInfo, PubInfo pubInfo)
  {
  	System.out.println(":j: *************************************************");
  	System.out.println(":j: * call method 'MyEvents.onPubRemoteNew()'..     *");
  	System.out.println(":j: *************************************************");
    // 
  	appInfo.printWithLegend();
  	pubInfo.printWithLegend();
  }

  public void onPubRemoteChanged(AppInfo appInfo, PubInfo pubInfo)
  {
  	System.out.println(":j: *************************************************");
  	System.out.println(":j: * call method 'MyEvents.onPubRemoteChanged()'.. *");
  	System.out.println(":j: *************************************************");
    // 
  	appInfo.printWithLegend();	
    pubInfo.printWithLegend();
  }

  public void onPubDelete(AppInfo appInfo, PubInfo pubInfo)
  {
  	System.out.println(":j: *************************************************");
  	System.out.println(":j: * call method 'MyEvents.onPubDelete()'..        *");
  	System.out.println(":j: *************************************************");
    // 
  	appInfo.printWithLegend();	
    pubInfo.printWithLegend();
  }
  
  
  public void onSubRemoteNew(AppInfo appInfo, SubInfo subInfo)
  {
  	System.out.println(":j: *************************************************");
  	System.out.println(":j: * call method 'MyEvents.onSubRemoteNew()'..     *");
  	System.out.println(":j: *************************************************");
    // 
  	appInfo.printWithLegend();	
    subInfo.printWithLegend();
  }

  public void onSubRemoteChanged(AppInfo appInfo, SubInfo subInfo)
  {
  	System.out.println(":j: *************************************************");
  	System.out.println(":j: * call method 'MyEvents.onSubRemoteChanged()'.. *");
  	System.out.println(":j: *************************************************");
    // 
  	appInfo.printWithLegend();	
    subInfo.printWithLegend();  
  }

  public void onSubDelete(AppInfo appInfo, SubInfo subInfo)
  {
  	System.out.println(":j: *************************************************");
  	System.out.println(":j: * call method 'MyEvents.onSubDelete()'..        *");
  	System.out.println(":j: *************************************************");
    // 
  	appInfo.printWithLegend();	
    subInfo.printWithLegend();
  }
  
}
