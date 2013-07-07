package org.ocera.orte.demo;

/* HelloMsg.java */

/* ********************************************************* *
 *                                                           *
 *                Will be Generated by IDL                   *
 *                                                           *
 * ********************************************************* */

import java.nio.ByteOrder;

import org.ocera.orte.*;
import org.ocera.orte.types.*;


public class HelloMsg extends MessageData
{
   private int counter = 0;
   public int count_read = 0; // pro testovaci ucely!!

/* ************************************** * 
 *               data to send             *
 * ************************************** */

   public  short 	  left;
   public  short 	  right;   
   
/* ************************************** */

   
  /* constructor  */
  public HelloMsg(DomainApp domainApp, String newTopic)
  {
    super();
    buffer.order(ByteOrder.LITTLE_ENDIAN);
 
    // typeName registration
	if(counter == 0)
    {
 	     boolean b;
		 
 	     counter++;
         this.setTopic(newTopic); // set the topic of a publication 
         // register new data typeName	   
     	 b = domainApp.regNewDataType("motion_speed",getMaxDataLength()); 
	     if (b == false) 
	     {
	       System.out.println(":j!: cannot register data type!");    
	     }

	   }
        // init
	    left = 0;
        right = 0;  
    	//System.out.println(":j: instance of HelloMsg created..");	
  }
      

  /**
   * Serializetaion method. Put all of data fields and write them into a buffer. 
   * @return True - if write successful, False - write failed (IndexOutOfBoundsException or ReadOnlyBufferException).  
   */
  /* TODO osetrit vyjimky - mozna pak bude vracet typ boolean 
   * IndexOutOfBoundsException - If index is negative or not smaller than the buffer's limit 
   * ReadOnlyBufferException - If this buffer is read-only
   */ 
  public void write()
  {
  	/* TODO - control setting fields !!*/
  	//
  	buffer.rewind(); // set offset to 0
  	buffer.putShort(this.left);
  	buffer.putShort(this.right);  	
  }
 
  /**
   * Serializetaion method. Put all of data fields and write them into a buffer. 
   * @return True - if write successful, False - write failed (IndexOutOfBoundsException or ReadOnlyBufferException).  
   */
  /* TODO 'BufferUnderflowException' - osetrit vyjimky - mozna pak bude vracet typ boolean 

   */ 
  public void read()
  {
   	buffer.rewind(); // set offset to 0
  	this.left = buffer.getShort();  			
  	this.right = buffer.getShort();
  }

  /**
   * Return maximum buffer's length according to its data fields.
   * @return Length of buffer.
   **/  
  public int getMaxDataLength()
  {
  	int len=0;
  	len += ORTEConstant.SHORT_FIELD_SIZE;
  	len += ORTEConstant.SHORT_FIELD_SIZE;
    return len;
  }

  public String toString()
  {
    String data = new String();
    
    data += (" left = " + left); 
    data += (" right = " + right);	
   	return data;	  
  }
  
}  