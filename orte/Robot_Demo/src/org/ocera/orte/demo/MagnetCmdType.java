package org.ocera.orte.demo;

import org.ocera.orte.DomainApp;
import org.ocera.orte.types.MessageData;
import org.ocera.orte.types.ORTEConstant;

public class MagnetCmdType extends MessageData {
	
	public int magnet = 0;

	  public MagnetCmdType(DomainApp domainApp, String newTopic) {
		    super();
		    this.setTopic(newTopic);
		    if (!domainApp.regNewDataType("magnet_cmd",getMaxDataLength())) {
	      System.out.println(":j!: cannot register data type \"magnet_cmd\"!");    
	    }
	  }
	  
	@Override
	public void read() {
	}

	@Override
	public void write() {
		buffer.rewind();
		buffer.putShort((short) magnet);
	}

	@Override
	public int getMaxDataLength() {
		return ORTEConstant.SHORT_FIELD_SIZE;
	}

}
