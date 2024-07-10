package org.ocera.orte.demo.DataTypes;

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
  public void read() {
	  buffer.rewind();
	  this.speed[0] = buffer.getShort();
	  this.speed[1] = buffer.getShort();
  }

  @Override
  public int getMaxDataLength()
  {
    return 2 * ORTEConstant.SHORT_FIELD_SIZE;
  }
}  
