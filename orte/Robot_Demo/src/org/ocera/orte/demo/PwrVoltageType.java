package org.ocera.orte.demo;

import org.ocera.orte.*;
import org.ocera.orte.types.*;


public class PwrVoltageType extends MessageData
{
   public  double[]	voltage = new double[4];
   
  public PwrVoltageType(DomainApp domainApp, String newTopic) {
    super();
    this.setTopic(newTopic);
    if (!domainApp.regNewDataType("pwr_voltage",getMaxDataLength())) {
      System.out.println(":j!: cannot register data type \"pwr_voltage\"!");    
    }
  }
      
  @Override
  public void write() {}
 
  @Override
  public void read() {
	  buffer.rewind();
	  for (int i = 0; i < 4; i++) {
		  voltage[i] = buffer.getDouble();
	  }
  }

  @Override
  public int getMaxDataLength() {
    return 4 * ORTEConstant.DOUBLE_FIELD_SIZE;
  }
}  
