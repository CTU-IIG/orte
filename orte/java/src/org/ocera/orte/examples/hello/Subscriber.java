/* Subscriber.java  */

/**
 * Class Subscriber provides methods for creating and working
 * with the ORTE subscriber.
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

import org.ocera.orte.*;
import org.ocera.orte.types.*;


public class Subscriber {

    public final static int IMMEDIATE    = 0x02;
    public final static int BEST_EFFORTS = 0x01;

   /* load native library 'libjorte.so' */
   static 
   {
     System.loadLibrary("jorte");      
   }


  public static void main(String[] args)
  {
    boolean b;

    System.out.println(":j: * ********************************************* *"); 
    System.out.println(":j: *         JORTE Subscriber is starting..        *");
    System.out.println(":j: * ********************************************* *");
    
    MyEvents events = new MyEvents();

    /**
     *  Create a new Application Domain for a Subscriber.
     * (Constructor without arguments creates the default Application Domain.)
     */ 
    // creates default app domain
    //DomainApp appDomain = new DomainApp();
    
    // creates user defined app domain
    DomainApp appDomain = new DomainApp(ORTEConstant.ORTE_DEFAULT_DOMAIN,
                                        DomainProp.defaultPropsCreate(),
                                        events,
                                        false);	
	if (appDomain == null) 
    {
      System.out.println(":!j:SUBS: cannot create application domain ");
      return;
    }
        
    // create a callback instance 
    MyCallback mycallback = new MyCallback();
    
    // create a message instance
    HelloMsg hellomsg = new HelloMsg(appDomain,"Example HelloMsg");

    // set subscriber's basic parameters
    NtpTime deadline      = new NtpTime(10);
    NtpTime minSeparation = new NtpTime(0);
    long multicastIPAddr  = 0;
    
    // create basic subscription properties
    SubsProp subProps = new SubsProp(hellomsg.getTopic(),
    		                         hellomsg.getType(),                       		
			                         minSeparation,  
			                         deadline,
									 IMMEDIATE,
									 BEST_EFFORTS,
									 multicastIPAddr);

    /** set verbosity mode - just uncomment the line bellow */
    // JOrte.verbositySetOptions("ALL:10");

    // create a subscription				                                             					                                             				                                             
    Subscription sub = appDomain.createSubscription(subProps,
	 		                                        hellomsg,
	 		                                        mycallback);

    if(sub == null) 
    {    
    	System.out.println(":!j: subscriber not created!");
        return;   
    }	
   	System.out.println(":j: subscriber created!");
    System.out.println(":j: * ********************************************* *");
   	System.out.println(":j: INFO: subscriber is setted to be alive for 10 counts");	
   	System.out.println(":j: * ********************************************* *");

   	// Subscriber is alive only 10 callback calling
	while(MyCallback.cnt < 11)
	{
	   System.out.println(":j: count: #" + MyCallback.cnt);
	   System.out.println(">>  recieved data: " + hellomsg);	    
	   JOrte.sleepMs(1000);
	   System.out.println(" ");
	}

	// kill & clean subscriber - call Garbage Collector 
	sub = null;
    System.out.println("Calling gc() ...");
	System.gc();

  }

}
