/** Domain.java */
package org.ocera.orte;

import  org.ocera.orte.types.*;
//import  org.ocera.orte.tools.CheckType;

/**
  * Class Domain provides methods for work with the ORTE domain.
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


public class Domain {

   /** load native library 'libjorte.so' */
   static {
      System.loadLibrary("jorte");
   }


   protected int               handle;
   public DomainProp           props;
   public DomainEvents         events;
   // je to tu spravne????????????????????????????????????????????????




  /**
   * constructor that creates default domain
   * domain = ORTE_DEFAULT_DOMAIN
   * suspended thread = false
   */
   public Domain()
   {
     // initialization of ORTE layer
     JOrte.init();
     System.out.println(":j: executed JOrte.init()..");
	 // this.handle =jORTEDomainDefaultAppCreate(ORTEConstant.ORTE_DEFAULT_DOMAIN,true); - uspane vlakno po startu
     this.handle =jORTEDomainDefaultAppCreate(ORTEConstant.ORTE_DEFAULT_DOMAIN,false);
     if(this.handle == 0) System.out.println(":j!: zero domain handle! ..");
   }


  /**
   * constructor
   *
   */
   public Domain(int domain, DomainProp ps, DomainEvents ev, boolean suspend)
   {

     // initialization of ORTE layer
     JOrte.init();
     System.out.println(":j: executed JOrte.init()..");
	 // set Default Domain Properties
     if(ps == null) {
         this.props = DomainProp.defaultPropsCreate();
     }
     else {
         this.props = ps;
     }
     // init Domain Events
     if(ev == null) {
         this.events = DomainEvents.init();
     }
     else {
         this.events = ev;
     }
     this.handle = jORTEDomainAppCreate(domain,ps.handle,ev.handle,suspend);
     if(this.handle == 0) System.out.println(":j!: zero domain handle! ..");
   }


  /**
   * destructor
   *
   */
   protected void finalize()
   {
     if(!destroyAllRegTypes()) System.out.println(":j!: destroyAllRegTypes fault..");
     if(!destroy()) System.out.println(":j!: ORTEDomainADestroy fault..");
   }


  /**
   * getHandle - returns a handle of the domain
   *
   * @return handler of the domain
   */
   public int getHandle()
   {
     return this.handle;
   }

  /**
   * getDomain - returns a handle of the domain
   *
   *
   * @param d given instance of Domain
   * @param jp_ORTEDomainProp address from C pointer
   * @return handler of the domain
   */
   public boolean destroy()
   {
     if (jORTEDomainADestroy(this.handle)) return true;
     return false;
   }

  /**
   * regNewDataType -
   *
   *
   * @param
   * @param
   * @param
   * @param
   * @return
   */
   public
   boolean regNewDataType(String name,
						// serial,  deserial,
							long maxlength)
   {
	int b = jORTETypeRegisterAdd(handle, name,
                 			//  serial, deserial,
					        maxlength);
	if (b == ORTEConstant.ORTE_BAD_HANDLE)
    {
  	 System.out.println(":!j: regNewDataType() - bad domain handle!");
  	 return false;
    }
    if (b == ORTEConstant.ORTE_OK)
    {
  	 System.out.println(":j: ORTE_OK - regNewDataType()..");
  	 return true;
	}

	return false;
   }



  /**
   * destroyAllRegTypes - destroys all previous registered data types.
   *
   *
   * @return boolean value, False when some error occures, otherwise True
   */
   public
   boolean destroyAllRegTypes()
   {
     if (jORTETypeRegisterDestroyAll(this.handle)) return true;
     return false;
   }


  /**
   * createSubscription -
   *
   *
   * @param
   * @param
   * @param
   * @param
   * @return
   */
   public
   Subscription createSubscription(int subsmode,
                                   int substype,
								   String topic,
                                   String typeName,
								   int bufflength,
                                   NtpTime deadline,
								   NtpTime minSeparation,
                                   SubscriptionCallback cb,
								/* recvCallBack */
								/* recvCallBackParam*/
								   long multicastIP)
   {
     Subscription s = new Subscription(this,
	                                   subsmode,
									   substype,
                                       topic,
									   typeName,
									   bufflength,
                                       deadline,
									   minSeparation,
									   cb,
									   multicastIP);
     return s;
   }




  /**
   * createPublication -
   *
   *
   * @param
   * @param
   * @param
   * @param
   * @return
   */
  public
  Publication createPublication (String topic,
                                 org.ocera.orte.types.MessageData instance,
								 NtpTime persistence,
	  				  	         int strength)
  {
  	System.out.println(":j: vytvarim publikaci pro DataType: " + instance.getType());
  	Publication p = new Publication(this,
									topic,
                                    instance,
									persistence,
									strength);
	return p;
  }



  /* ****************************************************************** *
   *                                                                    *
   *                         native methods                             *
   *                                                                    *
   * ****************************************************************** */

  /**
   * jORTEDomainAppCreate - creates an default application object within given
   * domain, returns handle to created object (from C environment),
   * NULL in case of any error.
   *
   *
   * @param domain given domain
   * @return addres value (from C environment) of the created domain
   */
   private static native
   int jORTEDomainDefaultAppCreate(int domain,boolean suspend);


  /**
   * jORTEDomainAppCreate - creates an application object within given
   * domain, returns handle to created object (from C environment),
   * NULL in case of any error.
   *
   *
   * @param domain given domain
   * @param handler for C pointer to ORTEDomainProps
   * @param handler for C pointer to ORTEDomainEvents
   * @param suspended specifies whether threads of this application should be started as well (False) or stay suspended (True)
   * @return addres value (from C environment) of the created domain
   */

   private static native
   int jORTEDomainAppCreate(int domain,
                            int propHandle,
                            int eventsHandle,
							boolean suspend);


  /**
   * jORTEDomainADestroy - destroys all application objects in specified
   * domain, returns False if some error occures otherwise returns True.
   *
   *
   * @param jp_domhandle handler to domain
   * @return boolean value, False when some error occures, otherwise True
   */
   private static native
   boolean jORTEDomainADestroy(int dhandle);


  /**
   * jORTEDomainStart -
   *
   * @param dhandle handler to domain
   * @param
   * @param
   * @return
   */
   private static native
   void jORTEDomainStart(int dhandle,
                         boolean recvMetatrafficThread,
                         boolean recvUserDataThread,
  	                     boolean sendThread);


  /**
   * jORTETypeRegisterAdd - register new data type.
   *
   *
   * @param dhandle handler to domain
   * @param
   * @param
   * @param
   * @return
   */
   private static native
   int jORTETypeRegisterAdd(int dhandle,
                            String typeName,
                         // serial,
						 // deserial,
                            long maxlenght);


  /**
   * jORTETypeRegisterDestroyAll - destroys all previous registered data types.
   *
   *
   * @param dhandle handler to domain
   * @return boolean value, False when some error occures, otherwise True
   */
   private static native
   boolean jORTETypeRegisterDestroyAll(int dhandle);


/* ****************************************************************** */





}

