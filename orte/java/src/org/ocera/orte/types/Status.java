/* Status.java */

/**
 * Class SequenceNumber substitutes the struct SequenceNumber
 * from C-source code defines in: 'typedefs_defines_rtps.h'
 *
 * @author Lukas Pokorny (lukas_pokorny@centrum.cz)
 * @author CTU FEE Prague - Department of Control Engineering (dce.felk.cvut.cz)
 * @author Project ORTE - OCERA Real Time Ethernet (www.ocera.org)
 * @author dedication to Kj
 * @version 0.1
 *
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

package org.ocera.orte.types;


public class Status {
   private long strict; 	
   private long bestEffort;	
   private long issues;

  /** 
   * Default constructor.
   */   
   public Status()
   {
      System.out.println(":j: instance of 'org.ocera.orte.types.Status' created..");	
   }

	/**
	 * Print actual fields state. Usable for example for check field.
	 */
	public String toString()
	{
	  return("Status - actual state: " + "strict = " + strict 
	  		 + " | bestEffort = " + bestEffort + " | issues = "
			 + issues);		
	}
   
// *** strict   
   public long getStrict()
   {
     return this.strict;	
   }

   
// *** bestEffort  
   public long getBestEffort()
   {
     return this.bestEffort;	
   }
   
   
// *** issues  
   public long getIssues()
   {
     return this.issues;	
   }
   
}
