/* Conversions.java */
package org.ocera.orte.tools;
import  org.ocera.orte.types.*;

/**
  * Class Conversions provides conversions between IPAddress
  * and String and between NtpTime and String.
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


public class Conversions {

   /** load native library 'libjorte.so' */
   static {
      System.loadLibrary("jorte");
   }


  /** native methods */

  /**
   * IPAddressToString - converts and return IP address from number
   * notation into its dot notation in string
   *
   * @param ipAddress source IP address in number notation
   * @return IP address in string(dot) notation
   */
   public static native
   String IPAddressToString(long ipAddress);


  /**
   * StringToIPAddress - converts IP and returns address from dot notation
   * -string into its number notation
   *
   * @param ipAddress source IP address in string(dot) notation
   * @return IP address in number notation
   */
   public static native
   long StringToIPAddress(String ipAddress);


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

