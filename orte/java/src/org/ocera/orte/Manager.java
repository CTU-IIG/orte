/** Manager.java */
package org.ocera.orte;
import org.ocera.orte.types.ORTEConstant;


/**
  * Class Manage provides methods for creating and working
  * with the ORTE manager.
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


public class Manager {

  private int handle;

  /**
   * getHandle - returns handler of the manager
   *
   * @return handler of the manager
   */
   public int getHandle()
   {
     return this.handle;
   }


  /**
   * constructor - that creates default Manager
   * domain = ORTE_DEFAULT_DOMAIN
   * suspended thread = false
   */
  public Manager ()
  {
    handle =
	jORTEDomainDefaultMgrCreate(ORTEConstant.ORTE_DEFAULT_DOMAIN,false);
  }


  /**
   * constructor
   *
   */
  public Manager (int domain,
                  DomainProp props,
				  DomainEvents events,
                  boolean suspend)
  {
    handle = jORTEDomainMgrCreate(domain,
	                              props.handle,
	                              events.handle,
								  suspend);
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
  * destroy - destroy manager object
  * @return boolean: False if bad publication handle, True if  succesful
  */
  public
  boolean destroy()
  {
    if(jORTEDomainMgrDestroy(this.handle)) return true;
    System.out.println(":j!: ORTEDomainMgrDestroy() fault..");
    return false;
  }


 /* ****************************************************************** *
  *                                                                    *
  *                         native methods                             *
  *                                                                    *
  * ****************************************************************** */


  /**
   * jORTEDomainMgrCreate - creates default Manager
   *
   *
   * @param handle of the domain
   * @return handle of the Manager
   */
   private static native
   int jORTEDomainDefaultMgrCreate(int dhandle,boolean suspend);


  /**
   * jORTEDomainMgrCreate - creates the Manager
   *
   *
   * @param handle of the domain
   * @param handle of the domain properties
   * @param handle of the domain events
   * @return handle of the Manager
   */
   private static native
   int jORTEDomainMgrCreate(int dhandle,
                            int propsHandle,
                            int eventsHandle,
                            boolean suspend);

  /**
   * jORTEDomainMgrDestroy - destroy manager object
   *
   *
   * @param hadle of the domain
   * @return if some error occures return False, otherwise True
   */
   private static native
   boolean jORTEDomainMgrDestroy(int dhandle);
}
