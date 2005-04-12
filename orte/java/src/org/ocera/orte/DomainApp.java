/* DomainApp.java */

/**
 * Class DomainApp provides methods for work with the ORTE application domain.
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
import  org.ocera.orte.types.*;


public class DomainApp extends Domain
{

  /**
   * Default constructor. Creates an Application Domain with default parameters.
   * (domain = ORTE_DEFAULT_DOMAIN, suspended thread = false)
   */
   public DomainApp()
   {
      super();
   	  // this.handle =jORTEDomainDefaultAppCreate(ORTEConstant.ORTE_DEFAULT_DOMAIN,true); - uspane vlakno po startu
      this.handle = jORTEDomainDefaultAppCreate(ORTEConstant.ORTE_DEFAULT_DOMAIN,false);
      if(this.handle == 0) System.out.println(":j!: zero domain handle! ..");   
        else System.out.println(":j: application domain created..");
   }

   
   /**
    * User defined constructor. For creating an Application Domain with your parameters.
    */
   /** TODO dodelat objekty 'props', 'events' */
   public DomainApp(int domain, DomainProp ps, DomainEvents ev, boolean suspend)
   {
     super(); 	 // set Default Domain Properties
     if(ps == null) {
         this.props = DomainProp.defaultPropsCreate();
     }
     else {
         this.props = ps;
     }
     // init Domain Events
     if(ev == null) {
         this.events.init();
     }
     else {
         this.events = ev;
     }
     this.handle = jORTEDomainAppCreate(domain,ps.handle,ev.getHandle(),ev,suspend);
     /* TODO osetrit neuspesne vytvoreni domeny */ 
     if(this.handle == 0) System.out.println(":j!: zero domain handle! ..");
      else System.out.println(":j: application domain created..");
   }

   
   /**
    * Destructor. Before it destroy Application Domain, destroy all registered 
    * data types too.
    */
   protected void finalize()
   {
     if(!destroyAllRegTypes()) System.out.println(":j!: destroyAllRegTypes fault!");
     if(!destroy()) System.out.println(":j!: ORTEDomainADestroy fault!");
   }

    
   /**
    * Destroy the Application Domain.  
    */
   public boolean destroy()
   {
     if (jORTEDomainAppDestroy(this.handle)) return true;
     return false;
   }

     
   /**
    * Register new data type.
    * @param Name of the new type.
    * @param Max length of the input buffer. 
    * @return True - registration Ok, False - registration fault. 
    */
   public
   boolean regNewDataType(String name,
   						  long maxlength)
   {
     int b = jORTETypeRegisterAdd(this.handle, 
     		                      name,
                                  maxlength);
   	 if (b == ORTEConstant.ORTE_BAD_HANDLE)
     {
       System.out.println(":!j: regNewDataType() failed! [bad domain handle]");
       return false;
     }
     if (b == ORTEConstant.ORTE_OK)
     {
       System.out.println(":j: type: '" + name + "' successfully registered..");
       return true;
   	 }
     return false;
   }


   /**
    * Destroys all previous registered data types.
    * @return True - registration Ok, False - destroying fault. 
    */
   public
   boolean destroyAllRegTypes()
   {
     if (jORTETypeRegisterDestroyAll(this.handle)) return true;
     return false;
   }


   /**
    * Creates a new Subscription.
    *
    * @param Subscription properties.
    * @param Length of output buffer.
    * @param Callback instance.
    * @return Handle to the new created Subscription.
    */
   public
   Subscription createSubscription(SubsProp subsProp,
                                    MessageData instance,
      		                        SubscriptionCallback subsCallback)
   {
       Subscription s = new Subscription(this,
                                         subsProp,
                                         instance,
                                         subsCallback);
       return s;
   }


   /**
    * Creates a new Publication.
    *
    * @param Instance of MessageData (which data will be send).
    * @param Time parameter, which defines how long are publicated data valid.
    * @param The importance of this Publication (its rating).
    * @return Handle to the new created Publication.
    */
   public
   Publication createPublication(PublProp publProp,
   		                      MessageData instance)
   {
     //System.out.println(":j: vytvarim publikaci pro DataType: " + instance.getType());
     Publication p = new Publication(this,
                                     publProp,
                                     instance);
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
 private static native int jORTEDomainDefaultAppCreate(int domain,boolean suspend);


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
						  DomainEvents ev,
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
 boolean jORTEDomainAppDestroy(int dhandle);

 
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

}
 