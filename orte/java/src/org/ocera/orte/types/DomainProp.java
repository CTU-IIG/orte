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
  public int handle = 0;


 /**
  * defaultPropsCreate - create DomainProp with handle to default
  * domain properties
  *
  * @return an object with handle to default domain properties
  */
  public static
  DomainProp defaultPropsCreate() {
     DomainProp prop = new DomainProp();
     int handle = jORTEDomainPropDefaultGet();
     prop.handle = handle;
     return prop;
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
  int jORTEDomainPropDefaultGet();

}

