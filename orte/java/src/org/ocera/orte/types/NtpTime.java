/* NtpTime.java */
package org.ocera.orte.types;


/**
  * Class NtpTime substitutes the struct NtpTime from
  * C-source code defines in: 'typedefs_defines_rtps.h'
  * 
  *   typedef struct {
  *     int32_t    seconds;    // time in seconds
  *     uint32_t   fraction;   // time in seconds / 2^32
  *   } NtpTime;
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
 
  /* TODO @fraction: v C pretypovat z long na uint32_t */

public class NtpTime {

  private int  seconds;  /* time in seconds        */
  private long fraction; /* time in seconds / 2^32 */


  /* constructors */

  /* NtpTime with fraction part */
  public NtpTime(int sec, long fract) {
    seconds  = sec;
    fraction = fract;
    System.out.println(":j: instance of NtpTime created..");
  }

  /* NtpTime with zero fraction part */
  public NtpTime(int sec) {
    seconds  = sec;
    fraction = 0;
    System.out.println(":j: instance of NtpTime created..");
  }

  /* toString */
  public String toString()
  {
    return("ntpTime: sec = " + seconds + " fraction = " + fraction);
  }


  /* get NtpTime */
  public NtpTime get()
  {
    return this;
  }
  /* set NtpTime */
  public void set(int s, long f)
  {
    seconds  = s;
	fraction = f;
    return;
  }



}


