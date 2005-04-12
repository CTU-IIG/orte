/* CheckType.java */

/**
 * Class CheckType checks the boundry of the types, which
 * substitutes the 'unsigned' types from C sources, because 
 * the types used in Java source code are longer than the 
 * unsigned types in C source code. For example type 'long'
 * [64bit] substitutes the type 'uint32_t' [32bit].
 * 
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

package org.ocera.orte.tools;


public class CheckType {

 /** constants - max and min boundry values of the unsigned integer32 */ 
   final static long UINT32MAX = (2^32 - 1);   
   final static long UINT32MIN = 0;


 /**
   * checkUINT_32 - check the boundry of the unsigned int32 type and 
   * return the bool value if the condition comes true or false
   *
   * @param number the value which will be checked
   * @return boolean value - true if the number is within the range of the unsigned int32 type
   */


    public static 
    boolean uint_32(long number) 
    {
     if (number >= UINT32MIN && number <= UINT32MAX) return(true);
     return(false);
    }

}

