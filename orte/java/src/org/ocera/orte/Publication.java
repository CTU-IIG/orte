/* Publication.java */

/**
 * Class Publication provides methods for creating and working
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

package org.ocera.orte;
import java.nio.ByteBuffer;

import org.ocera.orte.types.*;


public class Publication {

  private long handle;
  private org.ocera.orte.types.MessageData msgData;

  private boolean b;
  private Domain appDomain;
/*
  private int callbackEnvHandle = 0;
*/
  /**
   * getHandle - returns handler of the publication
   *
   * @return handler of the publication
   */
   public long getHandle()
   {
     return this.handle;
   }


  /**
   * constructor
   *
   */

  /* TODO   
   * nemelo by se v jORTEPublicationCreate misto cele instance predavat 
   * jen instance.data ????? resp instance.getData???? 
   * 
   * 
   */
   public Publication(Domain d,
                      PublProp publProp,
					  MessageData instance)
  {
   this.appDomain = d;
	this.handle = jORTEPublicationCreate(d.handle,
	                                     publProp.getTopic(),
	                                     publProp.getTypeName(),
										 instance.getBuffer(),
                                         publProp.getPersistence(),
									     publProp.getStrength());
                                      // int sendCallBack,
	                                  // sendCallBackParam,
                                      // NtpTime cbDelay
  }


 /**
  * destroy - Removes a publication.
  * @return False if bad publication handle, True if  succesful.
  */
  public
  boolean destroy()
  {
  	/* TODO vyradit vypis na nasledujici radce */
  	System.out.println(":j: publication destroy called..");
  	// destroy publication
  	if(!jORTEPublicationDestroy(this.handle)) 
  	{
  	    System.out.println(":j!: publication destroy fault!");
  	    return false;              	
  	}
  	// destroy application domain    
    if(!appDomain.destroy()) 
    {
  	    System.out.println(":j!: publication destroy fault!");
  	    return false;              	    	
    }
    System.out.println(":j: publication destroy successfull..");
    return true;
  }


 /**
  * send - Send the publication.
  * @return void
  */
  public void send(MessageData instance)
  {
    //this.msgData.write(); // volat v Publisher.java
    instance.write(); 

    b = jORTEPublicationSend(this.handle);
    if(!b) System.out.println(":j!: Sending Publication Fault!");
    return;
  }

  
  /**
   * Get Publication properties.
   * @return Publication properties.
   */
  public PublProp getProperties()
  {
  	PublProp sp = new PublProp();
    sp = jORTEPublicationPropertiesGet(this.handle);	
  	return sp;
  }
  
  /**
   * Set Publication properties.
   * @return True - setting new properties Ok, False - setting new properties fault.  
   */
  public boolean setProperties(PublProp sp)
  {
  	boolean  b;

  	// modify PublProp object - sp    
  	b = jORTEPublicationPropertiesSet(this.handle, sp);	
  	if(b == false) 
  	{
        System.out.println(":j!: set PublProp failed! (bad Subs. handle)"); 
  		return false; 
  	} 
  	return true;
  }
  
  
  /**
   * Get Publication status.
   * @return Status of Publication.  
   */
  public Status getStatus()
  {
    Status ps = new Status();
   	ps = jORTEPublicationGetStatus(this.handle);
  	return ps;
  }

  
  /**
   * Waits for given number of subscriptions. 
   * @param Time how long to wait.
   * @param Number of retries if specified number of subscriptions not reached.
   * @param Desired number of subscriptions.
   * @return ORTE_OK if succesful, ORTE_BAD_HANDLE if bad publication handle, 
   *         ORTE_TIMEOUT if number of retries has been exhausted.   
   */
  public int waitForSubscriptions(NtpTime wait,
  		                          long retries,
			     				  long noSubscription)   
  { 
    int i;
  	i = jORTEPublicationWaitForSubscriptions(this.handle,
  			                                 wait,
  			                                 retries,
											 noSubscription);
    return i; 
  }
  
  
 /* ****************************************************************** *
  *                                                                    *
  *                         native methods                             *
  *                                                                    *
  * ****************************************************************** */

  private native
  long jORTEPublicationCreate(long appDomainHandle,
                             String topic,
                             String typeName,
							 ByteBuffer buffer,
							 NtpTime persistence,
                             int strength);
                          // int sendCallBack, -->NULL
                          // sendCallBackParam,--> NULL
                          // NtpTime sendCallBackDelay

  private native
  boolean jORTEPublicationDestroy(long publHandle);

  private native
  boolean jORTEPublicationSend(long publHandle);
  		                       

  private native
  PublProp jORTEPublicationPropertiesGet(long publHandle);
  
  private native
  boolean jORTEPublicationPropertiesSet(long publHandle,
  		                                PublProp publProps);

  private native
  Status jORTEPublicationGetStatus(long publHandle);

  private native
  int jORTEPublicationWaitForSubscriptions(long publHandle,
  	                                       NtpTime wait,
	     								   long retries,
										   long noSubscription);

}
