/* JOrte.java */


package org.ocera.orte;

public class JOrte {


   public static
   void init()
   {
     jORTEInit();
   }

   public static
   void sleepMs(long msec)
   {
    jORTESleepMs(msec);
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
