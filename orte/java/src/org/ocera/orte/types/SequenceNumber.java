/* SequenceNumber.java */

/**
  * Class SequenceNumber substitutes the struct SequenceNumber
  * from C-source code defines in: 'typedefs_defines_rtps.h'
  *
  *  typedef struct {
  *      int32_t     high;
  *      int32_t     low;
  *  } SequenceNumber;
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


public class SequenceNumber
{
  private int    high;
  private long    low;


  /* constructor */
  public SequenceNumber(int high, long low)
  {
    this.high = high;
    this.low  = low;
    //System.out.println(":j: instance SequenceNumber created..");
  }

  public SequenceNumber get()
  {
    return this;
  }

  public long getDecimal()
  {
    long sn = high*2^32 + low; 
  	return sn;	
  }
  
}

