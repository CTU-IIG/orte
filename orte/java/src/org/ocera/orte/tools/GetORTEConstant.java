/* GetORTEConstant.java */
package org.ocera.orte.tools;


/**
  * Class GetORTEConstant is used for reading of constants defined
  * in C source code.
  *
  * @author Lukas Pokorny (lukas_pokorny@centrum.cz)
  * @author CTU FEE Prague - Department of Control Engineering (dce.felk.cvut.cz)
  * @author Project ORTE - OCERA Real Time Ethernet (www.ocera.org)
  * @author dedication to Kj
  * @version 0.1
  *
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



public class GetORTEConstant {

  /** name of the constant */
  String cName; 
  
  /** contructor */
  public GetORTEConstant(String name){
    cName = name;
  } 

  /** load native library 'libjorte.so' */
  static {
    System.loadLibrary("jorte");
  }


  /** nativ function */
  
  /** getCConstant - read the constant defined in C source code
   * 
   * @param name original name of the constant 
   * @return int value of the constant, -1 if constant doesn't exists 
   */
  public native int getConstant(String name);

  
  /* test */
  public static void main(String[] args) {
    int          c; 
    GetORTEConstant gc = new GetORTEConstant("RTPS_HEADER_LENGTH");
    
    c = gc.getConstant("RTPS_HEADER_LENGTH");
    System.out.println("delka je: " + c);
  } 

}

