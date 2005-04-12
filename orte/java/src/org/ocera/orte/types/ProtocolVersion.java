/* ProtocolVersion.java */

/**
  * Class ProtocolVersion substitutes the struct ProtocolVersion
  * from C-source code defines in: 'typedefs_defines_rtps.h'
  *
  *  typedef struct {
  *     int8_t        major;
  *     int8_t        minor;
  *   } ProtocolVersion;
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

package org.ocera.orte.types;


public class ProtocolVersion {

   byte           major;
   byte           minor;

   /* constructor */
   public ProtocolVersion(byte major,byte minor) 
   {
      this.major = major;
      this.minor = minor;
   }

  public String toString()
  {
  	return (major + "." + minor); 
  }
}
