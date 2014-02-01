package org.ocera.orte.demo.DataTypes;

import org.ocera.orte.DomainApp;
import org.ocera.orte.types.MessageData;
import org.ocera.orte.types.ORTEConstant;

public class CraneCmdType extends MessageData {
	
	public int position = 0;

	  public CraneCmdType(DomainApp domainApp, String newTopic) {
		    super();
		    this.setTopic(newTopic);
		    if (!domainApp.regNewDataType("crane_cmd",getMaxDataLength())) {
	      System.out.println(":j!: cannot register data type \"lift_cmd\"!");    
	    }
	  }
	  
	@Override
	public void read() {
	}

	@Override
	public void write() {
		buffer.rewind();
		buffer.putShort((short) position);
		buffer.putShort((short) 0);
	}

	@Override
	public int getMaxDataLength() {
		return 2*ORTEConstant.SHORT_FIELD_SIZE;
	}

}
