/* Subscription.java */
package org.ocera.orte;
import  org.ocera.orte.types.NtpTime;

/**
  * Class JORTEPublisher provides methods for creating and working
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


public class Subscription {

   private int handle;
   private int callbackContextHandle = 0;


  /**
   * getHandle - returns handler of the subscription
   *tCommon: root updated, wakeup
1103482260.672 | WakeUpSendin
   * @return handler of the subscription
   */
   public int getHandle()
   {
     return this.handle;
   }


  /**
   * constructor
   * PROC protected??? - aby sel volat jen z tridy Domain
   */
  protected Subscription(Domain d,
                         int subsmode,
		                 int substype,
                         String topic,
  		                 String typeName,
		                 int bufflength,
                         NtpTime deadline,
  		                 NtpTime minSeparation,
                         SubscriptionCallback cb,
	                  /* callBackParam */
					     long multicastIPAddress)
  {
    this.handle = jORTESubscriptionCreate(d.handle,
	                                      subsmode,
			  	                          substype,
                                          topic,
					                      typeName,
					                      bufflength,
                                          deadline,
					                      minSeparation,
					                      cb,
                                       /* callBackParam */  
										  multicastIPAddress);
  }


  /**
   * destructor
   *
   */
   protected void finalize()
   {
     destroy();
   }


 /**
  * destroy - removes a publication
  * @return boolean: False if bad publication handle, True if  succesful
  */
  protected
  boolean destroy()
  {
    System.out.println(":j!: ORTEPublicationDestroy() called..");
    if(jORTESubscriptionDestroy(this.handle)) return true;
    System.out.println(":j!: ORTEPublicationDestroy() fault..");
    return false;
  }


/* ****************************************************************** *
 *                                                                    *
 *                         native methods                             *
 *                                                                    *
 * ****************************************************************** */

  public native
  int jORTESubscriptionCreate(int dhandle,
                              int subsmode,  /*! union */
                              int substype,  /*! union */
                              String topic,
                              String typeName,
                              int bufflength,
                              NtpTime deadline,
                              NtpTime minSeparation,
                              SubscriptionCallback cb,
                            /* recvCallBackParam */
                              long multicastIPAddress);

 private native
 boolean jORTESubscriptionDestroy(int dhandle);


  /* native 'get' methods */
  public static native String    getTopic();
  public static native String    getTypeName();
  public static native NtpTime   getDeadline();
  public static native NtpTime   getMinSeparation();

}

