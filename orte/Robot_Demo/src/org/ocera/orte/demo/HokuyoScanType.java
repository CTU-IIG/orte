package org.ocera.orte.demo;

import org.ocera.orte.DomainApp;
import org.ocera.orte.types.MessageData;
import org.ocera.orte.types.ORTEConstant;

public class HokuyoScanType extends MessageData {
	
	public  int[]	hokuyo = new int[681];

	public HokuyoScanType(DomainApp domainApp, String newTopic) {
	    super();
            boolean b;
            this.setTopic(newTopic); 
            b = domainApp.regNewDataType("hokuyo_scan",getMaxDataLength()); 
            if (b == false) {
	      System.out.println(":j!: cannot register data type!");    
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

	public String toString() {
	  String data = new String();
	  for(int i = 0; i < hokuyo.length; i++) {
	    data += i + ": " + hokuyo[i] + ", ";
	  }
	  data = data.substring(0, data.length()-2);
	  return data;	  
	}
}
