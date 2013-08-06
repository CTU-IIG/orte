package org.ocera.orte.demo;

import org.ocera.orte.*;
import org.ocera.orte.types.*;


public class SpeedMotionType extends MessageData
{
   public  short[]	speed = new short[2];
   
  public SpeedMotionType(DomainApp domainApp, String newTopic) {
    super();
    this.setTopic(newTopic);
    if (!domainApp.regNewDataType("motion_speed",getMaxDataLength())) {
      System.out.println(":j!: cannot register data type \"motion_speed\"!");    
    }
  }
      
  @Override
  public void write()
  {
  	buffer.rewind();
  	buffer.putShort(this.speed[0]);
  	buffer.putShort(this.speed[1]);  	
  }
 
  @Override
  public void read() {}

  @Override
  public int getMaxDataLength()
  {
    return 2 * ORTEConstant.SHORT_FIELD_SIZE;
  }

  public String toString()
  {
    String data = "";
    
    data += (" left = " + speed[0]); 
    data += (" right = " + speed[1]);	
   	return data;	  
  }
  
}  
