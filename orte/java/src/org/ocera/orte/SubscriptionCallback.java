/* SubscriptionCallback.java */
package org.ocera.orte;
import  org.ocera.orte.types.*;


public class SubscriptionCallback //extends OrteCallback
{

  //public void callback()
  //public void callback(RecvInfo info, Object instance)
  public void callback(RecvInfo info)
  {
    System.out.println(":j: ****** SubscriptionCallback.callback ******");
  }

}
