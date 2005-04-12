/* MyCallback.java */

/**
 * Class Mycallback.
 * 
 * @author Lukas Pokorny (lukas_pokorny@centrum.cz)
 * @author CTU FEE Prague - Department of Control Engineering (dce.felk.cvut.cz)
 * @author Project ORTE - OCERA Real Time Ethernet (www.ocera.org)
 * @author dedication to Kj
 * @version 0.1
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

import org.ocera.orte.types.MessageData;
import org.ocera.orte.types.RecvInfo;
//import org.ocera.orte.types.SequenceNumber;

/**
 * @author pokorl
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */

class MyCallback extends org.ocera.orte.SubscriptionCallback
{
    static public int cnt = 0;
	int i;
	int num;
    //SequenceNumber sn; // only for test purposes

	/** constructor */
    public MyCallback()
	{
      // init
      i = 0; 
	  num = 0;
	  System.out.println(":j: MyCallback created..");
	}

    /** CallBack method
     * @param recieved info
     * @param message data instance 
     * @return void
     * */
    public void callback(RecvInfo info, MessageData msg)
	{
      //System.out.println(":j: MyCallback: new data received!");
	  cnt++;
	}


}
