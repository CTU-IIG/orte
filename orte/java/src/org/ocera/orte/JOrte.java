/* JOrte.java */

/**
 * Class JOrte prodies basic method for work with ORTE.
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

package org.ocera.orte;


public class JOrte {

   public static
   void init()
   {
     jORTEInit();
     System.out.println(":j: JORTE initialized..");
   }

   public static
   void sleepMs(long msec)
   {
    jORTESleepMs(msec);
    System.out.println(":j: sleep " + msec + "[milisec]  ..ZZzzzz..."); 
   }

   public static
   void verbositySetOptions(String opt)
   {
    jORTEVerbositySetOptions(opt);
   }

   public static
   void verbositySetLogFile(String logfile)
   {
    jORTEVerbositySetLogFile(logfile);
   }

   public static
   void appSendThread(int dhandle)
   {
    jORTEAppSendThread(dhandle);
   }



  /* ****************************************************************** *
   *                                                                    *
   *                         native methods                             *
   *                                                                    *
   * ****************************************************************** */


  /**
   * jORTEInit - initialization of ORTE layer
   *
   *
   * @return void
   */
   private static native
   void jORTEInit();


  /**
   * jORTESleepMs - suspend calling thread for given time in ms
   *
   *
   * @param d given time in milisec.
   * @return void
   */
   private static native
   void jORTESleepMs(long msec);


  /**
   * jORTEVerbositySetOptions -
   *
   *
   * @return void
   */
   private static native
   void jORTEVerbositySetOptions(String opt);


  /**
   * jORTEVerbositySetLogFile -
   *
   *
   * @return void
   */
   private static native
   void jORTEVerbositySetLogFile(String logfile);


  /**
   * jORTEVerbositySetLogFile -
   *
   *
   * @return void
   */
   private static native
   void jORTEAppSendThread(int dhandle);


}
