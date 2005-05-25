/* Publisher.java */

/**
 * Class Publisher provides methods for creating and working
 * with the ORTE publisher.
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


public class Publisher {

   // load native library 'libjorte.so'
   static 
   {
   	 System.loadLibrary("jorte");      
   }

  public static void main(String[] args)
  {

    System.out.println(":j: * ********************************************* *"); 
    System.out.println(":j: *         JORTE Publisher is starting..         *");
    System.out.println(":j: * ********************************************* *");
  	
  	// set publisher's parameters
    NtpTime persistence = new NtpTime(3);
    int strength = 1;
    // 
    MyEvents events = new MyEvents();
    
    /**
     *  Create a new Application Domain for a Publisher.
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
      System.out.println(":!j: cannot create default domain!");
      return;
	}
    
    // create a HelloMessage instance 
	HelloMsg hellomsg = new HelloMsg(appDomain,"Example HelloMsg");
    
    // set basic subscription properties
    PublProp publProp = new PublProp(hellomsg.getTopic(),
    		                         hellomsg.getType(),                       		
                                     persistence,
									 strength);
    
    System.out.println(":j: set publication properties..");		

    /* set verbosity mode - just uncomment the line bellow */
    // JOrte.verbositySetOptions("ALL:10");

	// create a publication    
    Publication pub = appDomain.createPublication(publProp,
    		                                      hellomsg);	        
    if (pub == null)
    {
      System.out.println(":j!: publisher not created!");
      return;
    }
    System.out.println(":j: publisher created!");
    System.out.println(":j: * ********************************************* *");    	
    System.out.println(":j: start sending data:");
    System.out.println(":j: * ********************************************* *");
    
    while(true)
	{
	  pub.send(hellomsg); 
	  System.out.println("<<  data to send: " + hellomsg);
	  JOrte.sleepMs(1000);
	  System.out.println(" ");
	}
  } 

}
