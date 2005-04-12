/* SubProp.java */

/**
 * Class SubProp.
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


public class SubsProp {
    private String     topic;     
    private String     typeName;  
    private int        typeChecksum;       
    private NtpTime    minSeparation; 
    private long       recvQueueSize;
    private long       reliabilityRequested;    
	// additional parameters
    private NtpTime    deadline; 
    private int        mode; // 0x01 = PULLED, 0x02 = IMMEDIATE    
    private int        type; // 0x01 = BEST_EFFORT, 0x02 = SRICT_RELIABLE
    private long       multicastIPAddr; 
	
	/** 
	 * Default constructor.
	 */
	public SubsProp() {
		//System.out.println(":j: instance SubsProp created..");	   
	}

	/**
	 *  User constructor. WARNING: This constructor is used for creating 
	 *  a new Subscriber - NOT ALL instance fields are initialize here.  
	 */
	public SubsProp(String  topic,     
                    String  typeName,  
                    NtpTime minSeparation, 
                    NtpTime deadline, 
                    int     mode,    
                    int     type,
                    long    multicastIPAddr) 
    {
		this.topic = topic;     
        this.typeName = typeName;  
        this.minSeparation = minSeparation; 
        this.deadline = deadline; 
        this.mode = mode;    
        this.type = type;
        this.multicastIPAddr = multicastIPAddr; 
	
		//System.out.println(":j: instance SubsProp created..");	   
	}


	/**
	 *  User-full constructor. ALL instance fields are initialize here.
	 */
	public SubsProp(String  topic,     
                    String  typeName,  
                    int     typeChecksum,       
                    NtpTime minSeparation, 
                    long    recvQueueSize,
                    long    reliabilityRequested,    
	                NtpTime deadline, 
                    int     mode,    
                    int     type,
                    long    multicastIPAddr) 
    {
		this.topic = topic;     
        this.typeName = typeName;  
        this.typeChecksum = typeChecksum;
        this.minSeparation = minSeparation; 
        this.recvQueueSize = recvQueueSize;
        this.reliabilityRequested = reliabilityRequested;     
        this.deadline = deadline; 
        this.mode = mode;    
        this.type = type;
        this.multicastIPAddr = multicastIPAddr; 
	
		//System.out.println(":j: instance SubsProp created..");	   
	}

	

	/**
	 * Print actual fields state. Usable for example for check field.
	 */
	public String toString()
	{
	  return("SubsProp - actual state: topic = " + topic + " | typeName = " + 
	  		  typeName + " | typeChecksum = " + typeChecksum + " | minSeparation: " + minSeparation
			  + " | recvQueueSize = " + recvQueueSize + 
			  " | reliabilityRequested = " + reliabilityRequested + " | deadline: " + 
 		      deadline + " | mode = " + mode + " | multicastIPAddr = " + multicastIPAddr);			
	}
  
	
//	*** topic	
	public String getTopic() 
	{
	   return this.topic;	
    }
	
	public void setTopic(String newTopic) 
	{
	   this.topic = newTopic;
	   return;	
    }


//	*** typeName	
	public String getTypeName() 
	{
	   return this.typeName;	
    }
	
	public void setTypeName(String newTypeName) 
	{
	   this.typeName = newTypeName;
	   return;	
    }

	
//	*** typeChecksum	
	public int getTypeChecksum() 
	{
	   return this.typeChecksum;	
    }
	
	public void setTypeChecksum(int newTypeChecksum) 
	{
	   this.typeChecksum = newTypeChecksum;
	   return;	
    }

	/**
	 * Return a property minSeparation.
	 * @return minSeparation
	 **/ 
	public NtpTime getMinSeparation() 
	{
	   return this.minSeparation;	
    }

		
	public void setMinSeparation(NtpTime newMinSeparation) 
	{
	   this.minSeparation = newMinSeparation;
	   return;	
    }
	
			
//	*** recvQueueSize
	public long getRecvQueueSize() 
	{
	   return this.recvQueueSize;	
    }

		
	public void setRecvQueueSize(long newRecvQueueSize) 
	{
	   this.recvQueueSize = newRecvQueueSize;
	   return;	
    }

	
//	*** reliabilityRequested
	public long getReliabilityRequested() 
	{
	   return this.reliabilityRequested;	
    }

		
	public void setReliabilityRequested(long newReliabilityRequested) 
	{
	   this.reliabilityRequested = newReliabilityRequested;
	   return;	
    }

	
//	*** deadline
	public NtpTime getDeadline() 
	{
	   return this.deadline;	
    }

		
	public void setDeadline(NtpTime newDeadline) 
	{
	   this.deadline = newDeadline;
	   return;	
    }

	
//	*** mode
	public int getMode() 
	{
	   return this.mode;	
    }
	
	
	public void setMode(int newMode) 
	{
	   this.mode = newMode;
	   return;	
    }
	
	
// *** type
	public int getType() 
	{
	   return this.type;	
    }

	
	public void setType(int newType) 
	{
	   this.type = newType;
	   return;	
    }


//	*** multicast
	public long getMulticastIPAddr() 
	{
	   return this.multicastIPAddr;	
    }

		
	public void setMulticastIPAddr(long newMulticastIPAddr) 
	{
	   this.multicastIPAddr = newMulticastIPAddr;
	   return;	
    }

}
