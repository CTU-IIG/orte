/* MessageData.java */

/*
Rozhranni pro objekty generovane pomoci IDLE.



*/
package org.ocera.orte.types;

public class MessageData
{
  //-- new
  private String type;
  private int    maxLength;
  // new --

  protected	byte[] data;

  public String getType()
  {
    char    extensionSeparator = '.';
    // get class name
	String  className = getClass().getName();
    // find last separator
	int     dot = className.lastIndexOf(extensionSeparator);
	return  className.substring(dot + 1);
  }

  public int  getMaxSerializeLength() {
    if(data == null)
	{
      System.out.println(":!j: NULL Message Data!");
	  return 0;
    }
	//return(data.size());
    return(maxLength);
  }

  public void  read() {};
  public void write() {};

/*
  // pro test 'name = getClass().getName()'
  public static void main(String[] args)
  {
     MessageData md = new MessageData();
	 System.out.println("nazev tridy: " + md.getType());
  }
*/

}



