/** Subscriber.java  */
import org.ocera.orte.*;
import org.ocera.orte.types.*;



/**
  * Class Publisher provides methods for creating and working
  * with the ORTE publisher.
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

class MyCallback extends org.ocera.orte.SubscriptionCallback
{
    int i;
	int num;
    SequenceNumber sn; // only for test purposes

	//public void callback(RecvInfo info, Object instance)
    public MyCallback()
	{
      System.out.println(":j: objekt MyCallback vytvoren..");
      i = 0; // init count
	  num = 0; // init
	}

    //public void callback()
    //public void callback(RecvInfo info)
	public void callback(RecvInfo info, Object instance)
	{
          num = info.i;
		  //System.out.println("Call back: " + obj);
          System.out.println("->JAVA");
		  System.out.println(":j: callback metoda: jsem tady!!!!!!!!!!!");
          System.out.println(":j: JAVA Call back " + (i++));
          System.out.println(" ");
		  System.out.println(":j: ---- RecvInfo [prijate]:");
		  System.out.println(":j:       pomocna: i = " + num);// only for test purposes
          System.out.println(":j:    recvStatus: " + info.getRecvStatus());// only for test purposes
		  System.out.println(":j:         topic: " + info.getTopic()); // only for test
          //info.type = "ahoj ["+ i + "]";
		  System.out.println(":j:          type: " + info.getType()); //
		  System.out.println(":j:    senderGuid: " + info.getSenderGuid()); //
          System.out.println(":j: localTimerecv: " + info.getLocalTimeRecv()); //
          System.out.println(":j: remoteTimePub: " + info.getRemoteTimePub()); //
          sn = info.getSeqNumber();
          System.out.println(":j:         seqNr: high = " + sn.high + " low = " + sn.low); // only for test purposes
		  System.out.println("/JAVA");
		  cnt++;

	}

	static public int cnt = 0;
}

public class Subscriber {

    public final static int IMMEDIATE    = 0x02;
    public final static int BEST_EFFORTS = 0x01;

   /** load native library 'libjorte.so' */
   static {
   	System.loadLibrary("jorte");      
   }





  public static void main(String[] args)
  {
    boolean      b;

    // create a new domain
    // (constructor without arguments creates default domain)
	org.ocera.orte.Domain od = new org.ocera.orte.Domain();


    if (od == null) {
      System.out.println(":!j:SUBS: PropDefaultGet() failed.. ");
    }
    else {
      System.out.println(":j:SUBS: PropDefaultGet() succesful " + od.props);
    }



	// register new data type
    b = od.regNewDataType("HelloMsg"/*,ser,deser*/,64);


    // set subscriber's time parameter
    NtpTime deadline      = new NtpTime(10);
    NtpTime minSeparation = new NtpTime(0);


    System.out.println(":j:SUBS: chystam se vytvorit subscribera");

    // create subscriber
    System.out.println(":j: Subscriber: chystam se vytvorit objekt MyCallback..");
	MyCallback mycallback = new MyCallback();
	//Subscription sub = od.initSubscription(Subscription sub, IMMEDIATE,BEST_EFFORTS,
    Subscription sub = od.createSubscription(IMMEDIATE,
                                             BEST_EFFORTS,
                                             "Example HelloMsg",
                                             "HelloMsg",
					                          64,
					                         deadline,
                                             minSeparation,
                     					     mycallback, 
			                              /* sendCallBackParam,*/
											 0);


	while(MyCallback.cnt < 5) {
	    System.out.println("MyCallback.cnt: " + MyCallback.cnt);
	    JOrte.sleepMs(1000);
	}
	sub = null;
    System.out.println("Calling gc() ...");
	System.gc();

  }

}
