/* PublProp.java */

/**
 * Class PublProp.
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


public class PublProp {
    private String     	topic;     
    private String     	typeName;  
    private int       	typeChecksum;       
    private boolean		expectsAck;
    private NtpTime     persistence; 
    
    private long        reliabilityOffered;
    private long        sendQueueSize;    
    private int         strength;             
    private long        criticalQueueLevel;                             
	
    private NtpTime     HBNornalRate;     
    private NtpTime     HBCQLRate;
    private long        HBMaxRetries;
    private NtpTime     maxBlockTime;
	
	/** 
	 * Default constructor.
	 */
   	public PublProp() 
   	{
    	//System.out.println(":j: instance of 'org.ocera.orte.types.PublProp' created..");	   
	}

	/**
	 *  User constructor. Warnig: This constructor is used for creating 
	 *  a new Publisher - NOT ALL instance fields are initialize here.  
	 */
	public PublProp(String  topic,     
                    String  typeName,  
                    NtpTime persistence, 
                    int     strength) 
    {
		this.topic = topic;     
        this.typeName = typeName;  
        this.persistence = persistence; 
        this.strength = strength;    	
        //System.out.println(":j: instance of 'org.ocera.orte.types.PublProp' created..");	   
	}


	/**
	 *  User-full constructor. ALL instance fields are initialize here.
	 */
	public PublProp(String    topic,     
                    String    typeName,  
                    int       typeChecksum,       
                    boolean   expectsAck, 
					NtpTime   persistence, 
					long      reliabilityOffered,
					long      sendQueueSize,
					int       strength,    
					long      criticalQueueLevel,	                
					NtpTime   HBNornalRate, 
                    NtpTime   HBCQLRate,
					int       HBMaxRetries,
                    NtpTime   maxBlockTime) 
    {
		this.topic = topic;     
		this.typeName = typeName;  
        this.typeChecksum = typeChecksum;
        this.expectsAck = expectsAck; 
        this.persistence = persistence; 
        
        this.reliabilityOffered = reliabilityOffered;
        this.sendQueueSize = sendQueueSize;
        this.strength = strength;
        this.criticalQueueLevel = criticalQueueLevel;  
        
        this.HBNornalRate = HBNornalRate; 
        this.HBCQLRate = HBCQLRate;
        this.HBMaxRetries = HBMaxRetries;
        this.maxBlockTime = maxBlockTime; 
	
        System.out.println(":j: instance of 'org.ocera.orte.types.PublProp' created..");	   
	}

	

	/**
	 * Print actual fields state. Usable for example for check field.
	 */
	public String toString()
	{
	  return("PublProp - actual state: topic = " + topic + " | typeName = " + 
	  		  typeName + " | typeChecksum = " + typeChecksum + " | expectsAck = " 
			  + expectsAck + " | persistence: " + persistence + 
			  " | reliabilityOffered = " + reliabilityOffered + 
			  " | sendQueueSize = " + sendQueueSize + " | strength = " + strength 
			  + " | criticalQueueLevel = " + criticalQueueLevel + " | HBNornalRate: "
			  + HBNornalRate + " | HBCQLRate: " + HBCQLRate + " | HBMaxRetries = " 
			  + HBMaxRetries + " | maxBlockTime: " + maxBlockTime);		
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

//	*** expectsAck	
	public boolean getExpectsAck() 
	{
	   return this.expectsAck;	
    }
	
	public void setExpectsAck(boolean newExpectsAck) 
	{
	   this.expectsAck = newExpectsAck;
	   return;	
    }


//  *** persistence	
	public NtpTime getPersistence() 
	{
	   return this.persistence;	
    }

		
	public void setPersistence(NtpTime newPersistence) 
	{
	   this.persistence = newPersistence;
	   return;	
    }
	
		
//	*** reliabilityOffered
	public long getReliabilityOffered() 
	{
	   return this.reliabilityOffered;	
    }

		
	public void setReliabilityOffered(long newReliabilityOffered) 
	{
	   this.reliabilityOffered = newReliabilityOffered;
	   return;	
    }

	
//	*** sendQueueSize
	public long getSendQueueSize() 
	{
	   return this.sendQueueSize;	
    }

		
	public void setSendQueueSize(long newSendQueueSize) 
	{
	   this.sendQueueSize = newSendQueueSize;
	   return;	
    }

	
//	*** strength
	public int getStrength() 
	{
	   return this.strength;	
    }
	
	public void setStrength(int newStrength) 
	{
	   this.strength = newStrength;
	   return;	
    }
	

//	*** criticalQueueLevel
	public long geCriticalQueueLevel() 
	{
	   return this.criticalQueueLevel;	
    }

		
	public void setCriticalQueueLevel(long newCriticalQueueLevel) 
	{
	   this.criticalQueueLevel = newCriticalQueueLevel;
	   return;	
    }
	
	
//	*** HBNornalRate
	public NtpTime getHBNornalRate() 
	{
	   return this.HBNornalRate;	
    }

		
	public void setHBNornalRate(NtpTime newHBNornalRate) 
	{
	   this.HBNornalRate = newHBNornalRate;
	   return;	
    }
	

//	*** HBCQLRate
	public NtpTime getHBCQLRate() 
	{
	   return this.HBCQLRate;	
    }

		
	public void setHBCQLRate(NtpTime newHBCQLRate) 
	{
	   this.HBCQLRate = newHBCQLRate;
	   return;	
    }

	
// *** HBMaxRetries
	public long getHBMaxRetries() 
	{
	   return this.HBMaxRetries;	
    }
	
	public void setHBMaxRetries(long newHBMaxRetries) 
	{
	   this.HBMaxRetries = newHBMaxRetries;
	   return;	
    }

//	*** maxBlockTime
	public NtpTime getMaxBlockTime() 
	{
	   return this.maxBlockTime;	
    }

		
	public void setMaxBlockTime(NtpTime newMaxBlockTime) 
	{
	   this.maxBlockTime = newMaxBlockTime;
	   return;	
    }

	


}

