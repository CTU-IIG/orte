/* DomainProp.java */

/**
 * Class DomainProp. 
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


public class DomainProp {

 /* load native library 'libjorte.so' */
  static {
     System.loadLibrary("jorte");
  }


 /* handler to C struct with default domain properties  */
  private long handle = 0;
  private String mgrs = null;
  
  public long getHandle() {
	  return this.handle;
  }

 /**
  *  setProps - sets DomainProp
  */
  public void setProps(String[] mgrs) {
          this.mgrs = "";
	  for (String item : mgrs) {
		  this.mgrs += item + ":";
	  }
	  this.mgrs = this.mgrs.substring(0, this.mgrs.length()-1);

	  if(!jORTEDomainPropSet(this.handle,this.mgrs))
		  System.out.println(":j: DomainProp not set !!!");
  }

 /**
  * defaultPropsCreate - create DomainProp with handle to default
  * domain properties
  *
  * @return an object with handle to default domain properties
  */
  public static
  DomainProp defaultPropsCreate() {
     DomainProp prop = new DomainProp();
     long handle = jORTEDomainPropDefaultGet();
     prop.handle = handle;
     return prop;
  }

  public boolean destroy() {
    System.out.println(":j: DomainProp destroy called..");

    if(!jORTEDomainPropDestroy(this.handle,this.mgrs)) {
      System.out.println(":j: DomainProp destroy fault!");
      return false;
    }
    else {
      System.out.println(":j: DomainProp destroy successful..");
      return true;
    }
  }


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
  long jORTEDomainPropDefaultGet();

  private native
  boolean jORTEDomainPropSet(long prophandle, String mgrs);

  private native
  boolean jORTEDomainPropDestroy(long prophandle, String mgrs);

}

