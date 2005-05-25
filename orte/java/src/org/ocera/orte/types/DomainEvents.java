/* DomainEvents.java */

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


public class DomainEvents {
  /* handler to C struct with default domain properties  */
  private int handle = 0;


 /* load native library 'libjorte.so' */
  static {
     System.loadLibrary("jorte");
  }

  public DomainEvents()
  {
  	init();
  	System.out.println(":j: events created & initialized..");
  }


  /**
   * Initializes DomainEvents and return their handle
   * @return handle initialized DomainEvents
   */
  public void init()
  {
    this.handle = jORTEDomainInitEvents();
  	return;
  }

  
  public int getHandle()
  {
  	return this.handle;
  }
  
  public void onRegFail() { }
  public void onMgrNew(AppInfo appInfo) { }
  public void onMgrDelete(AppInfo appInfo) { }
  public void onAppRemoteNew(AppInfo appInfo) { }
  public void onAppDelete(AppInfo appInfo) { }
  public void onPubRemoteNew(AppInfo appInfo, PubInfo pubInfo) { }
  public void onPubRemoteChanged(AppInfo appInfo, PubInfo pubInfo) { }
  public void onPubDelete(AppInfo appInfo, PubInfo pubInfo) { }
  public void onSubRemoteNew(AppInfo appInfo, SubInfo subInfo) { }
  public void onSubRemoteChanged(AppInfo appInfo, SubInfo subInfo) { }
  public void onSubDelete(AppInfo appInfo, SubInfo subInfo) { }
  
 /* ****************************************************************** *
  *                                                                    *
  *                         native methods                             *
  *                                                                    *
  * ****************************************************************** */


 /**
  * jPropDefaultGet - returns handler to default properties of a domain,
  * if occures some error return NULL.
  *
  * @return handler to default properties of a domain, NULL if error
  */
  private static native
  int jORTEDomainInitEvents();



}
