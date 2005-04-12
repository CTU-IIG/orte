/* NtpTime.java */

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

package org.ocera.orte.types;
 
  /* TODO @fraction: v C pretypovat z long na uint32_t */

public class NtpTime {

  private int  seconds;  /* time in seconds        */
  private long fraction; /* time in seconds / 2^32 */


  /**
   * Default constructor. 
   */
  public NtpTime() {
    //System.out.println(":j: instance of NtpTime created..");
  }

  /**
   * User constructor of NtpTime with zero fraction part.  
   */
  public NtpTime(int sec) {
    seconds  = sec;
    fraction = 0;
    //System.out.println(":j: instance of NtpTime created..");
  }
  
   /**
    * User constructor full defined.  
    */
  public NtpTime(int sec, long fract) {
    seconds  = sec;
    fraction = fract;
    //System.out.println(":j: instance of NtpTime created..");
  }

  /**
   * @return String with actual state of NtpTime instance.  
   */
  public String toString()
  {
    return("sec = " + seconds + " fraction = " + fraction);
  }

  /**
   * Get NtpTime. 
   * @return actual state of NtpTime instance
   **/
  public NtpTime get()
  {
    return this;
  }

  /**
   * Get NtpTime in its decimal form.
   * @return actual NtpTime's value
   **/
  public long getDecimal()
  {
    return this.seconds + (this.fraction/2^32);
  }
    
  /* ****************************************************************** *
   *                                                                    *
   *                         native methods                             *
   *                                                                    *
   * ****************************************************************** */


  /**
   * NtpTimeToStringMs - converts NtpTime from number notation into its
   * string notation in miliseconds
   *
   * @param time time given in JNtpTime object
   * @return NtpTime in string notation in miliseconds
   */
   public static native
   String NtpTimeToStringMs(NtpTime time);

   /**
   * NtpTimeToStringUs - converts NtpTime from number notation into its
   * string notation in microseconds
   *
   * @param time time given in JNtpTime object
   * @return NtpTime in string notation in microseconds
   */
   public static native
   String NtpTimeToStringUs(NtpTime time);
   
}