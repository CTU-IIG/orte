package org.ocera.orte.demo;

import org.ocera.orte.*;
import org.ocera.orte.types.*;


public class SpeedMotionType extends MessageData
{
   public  short[]	speed = new short[2];
   
  public SpeedMotionType(DomainApp domainApp, String newTopic) {
    super();
    boolean b;
    this.setTopic(newTopic);
    b = domainApp.regNewDataType("motion_speed",getMaxDataLength()); 
    if (b == false) {
      System.out.println(":j!: cannot register data type!");    
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
  	int len=0;
  	len += ORTEConstant.SHORT_FIELD_SIZE;
  	len += ORTEConstant.SHORT_FIELD_SIZE;
    return len;
  }

  public String toString()
  {
    String data = new String();
    
    data += (" left = " + speed[0]); 
    data += (" right = " + speed[1]);	
   	return data;	  
  }
  
}  
