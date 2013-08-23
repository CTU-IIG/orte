package org.ocera.orte.demo;

import org.ocera.orte.DomainApp;
import org.ocera.orte.types.MessageData;
import org.ocera.orte.types.ORTEConstant;

public class HokuyoScanType extends MessageData {
	
	public  int[]	hokuyo = new int[681];

	public HokuyoScanType(DomainApp domainApp, String newTopic) {
	    super();
	    this.setTopic(newTopic); 
	    if (!domainApp.regNewDataType("hokuyo_scan",getMaxDataLength())) {
	    	System.out.println(":j!: cannot register data type \"hokuyo_scan\"!");    
	    }
	}

	@Override
	public void read() {
		buffer.rewind();
		for (int i = 0; i < hokuyo.length; i++) {
		    this.hokuyo[i] = buffer.getShort() & 0xffff;
		}
	}

	@Override
	public void write() {}

	@Override
	public int getMaxDataLength() {
		return 681 * ORTEConstant.SHORT_FIELD_SIZE;
	}
}
