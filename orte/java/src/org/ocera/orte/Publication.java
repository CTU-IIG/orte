/** Publication.java */
package org.ocera.orte;

import org.ocera.orte.types.*;
//import org.ocera.tools.CheckType;

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


public class Publication {

  private int handle;
  private org.ocera.orte.types.MessageData msgData;

  private boolean b;
/*
  private int callbackEnvHandle = 0;
*/
  /**
   * getHandle - returns handler of the publication
   *
   * @return handler of the publication
   */
   public int getHandle()
   {
     return this.handle;
   }


  /**
   * constructor
   *
   */
  public Publication (Domain d,
                      String topic,
                      org.ocera.orte.types.MessageData instance,
					  NtpTime persistence,
					  int strength)
  {
	this.handle = jORTEPublicationCreate(d.handle,
	                                     topic,
				      					 instance.getType(),
                                         instance,
										 persistence,
									     strength);
                                      // int sendCallBack,
	                                  // sendCallBackParam,
                                      // NtpTime cbDelay									     
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
  * destroy - Removes a publication.
  * @return False if bad publication handle, True if  succesful.
  */
  public
  boolean destroy()
  {
    if(jORTEPublicationDestroy(this.handle)) return true;
    System.out.println(":j!: Destroy Publication Fault!");
    return false;
  }


 /**
  * send - Send the publication.
  * @return void
  */
  public void send(MessageData instance)
  {
    msgData.write();
    b = jORTEPublicationSend(this.handle,instance);
    if(!b) System.out.println(":j!: Sending Publication Fault!");
    return;
  }

 /* ****************************************************************** *
  *                                                                    *
  *                         native methods                             *
  *                                                                    *
  * ****************************************************************** */

  private native
  int jORTEPublicationCreate(int dhandle,
                             String topic,
                             String typeName,
                             org.ocera.orte.types.MessageData instance,
							 NtpTime persistence,
                             int strength);
                          // int sendCallBack, -->NULL
                          // sendCallBackParam,--> NULL
                          // NtpTime sendCallBackDelay


  private native
  boolean jORTEPublicationDestroy(int dhandle);


  private native
  boolean jORTEPublicationSend(int phandle, MessageData instance);



  /* 'get' methods */
/*  
  public static native String    getTopic();
  //public static native String    getTypeName();
  public static native NtpTime   getPersistance();
  public static native int       getStrength();
*/
}

