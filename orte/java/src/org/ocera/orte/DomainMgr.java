/* DomainMgr.java */

/**
 * Class DomainMgr provides methods for work with the ORTE manager domain.
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
import org.ocera.orte.types.DomainEvents;
import org.ocera.orte.types.DomainProp;
import org.ocera.orte.types.ORTEConstant;


public class DomainMgr extends Domain 
{

   /**
	* Default constructor. Creates default Manager domain.
	* (domain = ORTE_DEFAULT_DOMAI, suspended thread = false)
	**/
   public DomainMgr()
   {
     handle = jORTEDomainDefaultMgrCreate(ORTEConstant.ORTE_DEFAULT_DOMAIN,
     		                              false);
   }


   /**
	* User constructor. Creates manager domain with user parameters.
	* @param given domain
	* @param domain properties
	* @param domain events
	* @param specifies whether threads of thiss manager should be started as well (False) or stay suspended (True) 
	**/
    /* TODO dodelat instance 'props', 'events' */
    public DomainMgr(int domain,
	                 DomainProp props,
	                 DomainEvents events,
	                 boolean suspend)
    {
	   handle = jORTEDomainMgrCreate(domain,
	                                 props.handle,
		                             events.getHandle(),
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

	 /*
     public void create()
     {}
	 */
     
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
   * @param handle of the domain
   * @return handle of the Manager
   **/
   private static native
   int jORTEDomainDefaultMgrCreate(int dhandle,boolean suspend);


  /**
   * jORTEDomainMgrCreate - creates the Manager
   * @param handle of the domain
   * @param handle of the domain properties
   * @param handle of the domain events
   * @return handle of the Manager
   **/
   private static native
   int jORTEDomainMgrCreate(int dhandle,
                            int propsHandle,
                            int eventsHandle,
                            boolean suspend);

  /**
   * jORTEDomainMgrDestroy - destroy manager object
   * @param hadle of the domain
   * @return if some error occures return False, otherwise True
   **/
   private static native
   boolean jORTEDomainMgrDestroy(int dhandle);  
	
}
